#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H
#include <vector>
#include <mutex>
#include <unordered_map>
#include <cstddef>
#include "Core/Singleholder.h"
namespace PH {

class MemoryPoolBase {
public:
    explicit MemoryPoolBase();
    ~MemoryPoolBase();
    void *get();
    void release(void *ptr);
    std::size_t blockSize()const;
    int allocated()const;
    int available()const;
    void init(std::size_t blockSize,int preAlloc=0,int maxAlloc=0);
private:
    MemoryPoolBase(const MemoryPoolBase&)=delete;
    MemoryPoolBase& operator=(const MemoryPoolBase&)=delete;

    void clear();

    enum{
        BLOCK_RESERVE=128
    };

    typedef std::vector<char*> BlockVec;

    std::size_t _blockSize=128;
    int _maxAlloc=0;
    int _allocated=0;
    BlockVec _blocks;
    std::mutex _mutex;
};
class MemoryPool : public PH::SingleHolder<MemoryPool>{
public:
    MemoryPool();

    void *get(size_t nSize);

    void release(void *ptr);

    std::size_t blockSize(size_t nSize)const;
private:
    friend class PH::SingleHolder<MemoryPool>;
    MemoryPool(const MemoryPool&)=delete;
    MemoryPool& operator=(const MemoryPool&)=delete;

    typedef std::unordered_map<size_t,MemoryPoolBase*> MemoryMap;
    MemoryMap _memoryMap;
    std::mutex _mutex;
};

#define FAST_MEMORY_POOL_PRE_ALLOC 1000
template<typename T>
class FastMemoryPool{
private:
    class Block{
    public:
        Block(){
            _memory.next=this+1;
        }
        explicit Block(Block* next){
            _memory.next=next;
        }
        union {
            char buffer[sizeof(T)];
            Block* next;
        }_memory;
    private:
        Block(const Block&)=delete;
        Block& operator = (const Block&)=delete;
    };

public:
    typedef std::vector<Block*> BucketVec;
    FastMemoryPool(std::size_t blockPerBucket=FAST_MEMORY_POOL_PRE_ALLOC,std::size_t bucketPreAlloc=10,
                   std::size_t maxAlloc=0):
        _blocksPerBucket(_blocksPerBucket),
        _maxAlloc(maxAlloc),
        _available(0){
        if(_blocksPerBucket<2) throw std::invalid_argument("FastMemoryPool: blocksPerBucket must be >=2");
        _buckets.reserve(bucketPreAlloc);
        resize();
    }

    ~FastMemoryPool(){
        clear();
    }

    void* get(){
        Block* ret;
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if(_firstBlock==0) resise();
            ret=_firstBlock;
            _firstBlock=_firstBlock->_memory.next;
        }

        --_available;
        return ret;
    }

    template <typename P>
    void release(P* ptr){
        if(!ptr) return;
        reinterpret_cast<P*>(ptr->~P());
        ++_available;
        std::unique_lock<std::mutex> lock(_mutex);
        _firstBlock=new (ptr)Block(_firstBlock);
    }
    std::size_t blockSize()const{
        return sizeof(Block);
    }

    std::size_t allocated()const{
        return _buckets.size()*_blocksPerBucket;
    }

    std::size_t available()const{
        return _available;
    }

private:
    FastMemoryPool(const FastMemoryPool&)=delete;
    FastMemoryPool& operator=(const FastMemoryPool&)=delete;
    void resize(){
        if(_buckets.size()==_buckets.capacity()){
            std::size_t newSize=_buckets.capacity()*2;
            if(_maxAlloc !=0&&newSize >_maxAlloc) throw std::bad_alloc();
            _buckets.reserve(newSize);
        }

        _buckets.push_back(new Block[_blocksPerBucket]);
        _firstBlock = _buckets.back();
        _firstBlock[_blocksPerBucket-1]._memory.next = 0;
        _available = _available.load()+ static_cast<uint32_t>(_blocksPerBucket);
    }

    void clear(){
        typename BucketVec::iterator it=_buckets.begin();
        typename BucketVec::iterator end=_buckets.end();
        for(;it!=end;++it) delete[] *it;
    }

    typedef std::atomic_uint Counter;
    const std::size_t _blocksPerBucket;
    BucketVec _buckets;
    Block* _firstBlock;
    std::size_t _maxAlloc;
    Counter _available;
    mutable std::mutex _mutex;
};
inline std::size_t MemoryPoolBase::blockSize() const
{
    return _blockSize;
}


inline int MemoryPoolBase::allocated() const
{
    return _allocated;
}


inline int MemoryPoolBase::available() const
{
    return (int)_blocks.size();
}

}

#endif // MEMORYPOOL_H
