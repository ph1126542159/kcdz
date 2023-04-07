#include "MemoryPool.h"
#include <assert.h>
#include "Core/Exception.h"
namespace PH {

MemoryPoolBase::MemoryPoolBase(){
}


MemoryPoolBase::~MemoryPoolBase(){
    clear();
}

void MemoryPoolBase::clear(){
    for(auto p:_blocks){
        delete [] p;
    }
    _blocks.clear();
}


void* MemoryPoolBase::get(){
    std::unique_lock<std::mutex> lock(_mutex);
    if(_blocks.empty()){
        if(_maxAlloc==0||_allocated<_maxAlloc){
            ++_allocated;
            return new char[_blockSize];
        }else throw PH::Exception("Memory out");
    }else{
        char* ptr=_blocks.back();
        _blocks.pop_back();
        return ptr;
    }
}

void MemoryPoolBase::release(void* ptr){
    std::unique_lock<std::mutex> lock(_mutex);
    try{
        _blocks.push_back(reinterpret_cast<char*>(ptr));
    }catch(...){
        delete [] reinterpret_cast<char*>(ptr);
    }
}

void MemoryPoolBase::init(size_t blockSize, int preAlloc, int maxAlloc)
{
    assert(maxAlloc==0 || maxAlloc>=preAlloc);
    assert(preAlloc>=0 && maxAlloc>=0);
    _blockSize=blockSize;
    _allocated=preAlloc;
    _maxAlloc=maxAlloc;

    int r=BLOCK_RESERVE;
    if(preAlloc>r) r=preAlloc;

    if(maxAlloc>0&&maxAlloc<r) r=maxAlloc;
    _blocks.reserve(r);

    try {
        for(int i=0;i<preAlloc;++i){
            _blocks.push_back(new char[_blockSize]);
        }
    } catch (...) {
        clear();
        throw;
    }
}

MemoryPool::MemoryPool()
{

}

void *MemoryPool::get(size_t nSize)
{
    nSize=blockSize(nSize);

    std::unique_lock<std::mutex> lock(_mutex);
    if(nullptr==_memoryMap[nSize]){
        _memoryMap[nSize]=new MemoryPoolBase();
        _memoryMap[nSize]->init(nSize);
    }
    char* ptr=(char*)_memoryMap[nSize]->get();
    std::memset(ptr,0,nSize);
    std::memcpy(ptr,&nSize,4);
    return (void*)(ptr+4);
}

void MemoryPool::release(void *ptr)
{
    char* buffer=(char*)ptr;
    buffer=buffer-4;
    size_t nSize=0;
    std::memcpy(&nSize,buffer,4);

    std::unique_lock<std::mutex> lock(_mutex);
    _memoryMap[nSize]->release(buffer);
}

size_t MemoryPool::blockSize(size_t nSize) const
{
    nSize+=4;
    const unsigned long minBlock=32;
    size_t m=nSize/minBlock;
    size_t n=nSize%minBlock;
    if(n>0) m+=1;
    nSize=m*minBlock;
    return nSize;
}

}
