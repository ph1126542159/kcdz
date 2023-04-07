#ifndef BUFFER_H
#define BUFFER_H
#include "Exception.h"
#include <cstring>
#include <cstddef>

namespace PH {
template<class T>
class Buffer{
public:
    Buffer(std::size_t length):_capacity(length),_used(length),_ptr(nullptr),_ownMem(true){
        if(length>0) _ptr=new T[length];
    }

    Buffer(T* pMem,std::size_t length):_capacity(length),_used(length),_ptr(pMem),_ownMem(false){
    }

    Buffer(const T* pMem,std::size_t length):_capacity(length),_used(length),_ptr(nullptr),_ownMem(true){
        if(_capacity>0){
            _ptr=new T[_capacity];
            std::memcpy(_ptr,pMem,_used*sizeof(T));
        }
    }

    Buffer(Buffer&& other)noexcept:_capacity(other._capacity),_used(other._used),_ptr(other._ptr),_ownMem(other._ownMem){

    }

    Buffer& operator =(const Buffer& other){
        if(this!=&other){
            Buffer tmp(other);
            swap(tmp);
        }
        return *this;
    }

    Buffer& operator=(Buffer&& other)noexcept{
        if(_ownMem) delete [] _ptr;
        _capacity=other._capacity;
        _used=other._used;
        _ptr=other._ptr;
        _ownMem=other._ownMem;

        other._capacity=0;
        other._used=0;
        other._ptr=nullptr;
        other._ownMem=false;

        return *this;
    }

    ~Buffer(){
        if(_ownMem) delete [] _ptr;
    }

    void resize(std::size_t newCapacity,bool preserveContent=true){
        if(!_ownMem) PH::InvalidAccessException("Cannot resize buffer which does not own its storage.");

        if(newCapacity>_capacity){
            T* ptr=new T[newCapacity];
            if(preserveContent&&_ptr){
                std::memcpy(ptr,_ptr,_used*sizeof(T));
            }
            delete [] _ptr;
            _ptr = ptr;
            _capacity = newCapacity;
        }

        _used=newCapacity;
    }

    void setCapacity(std::size_t newCapacity,bool preserveContent=true){
        if (!_ownMem) throw PH::InvalidAccessException("Cannot resize buffer which does not own its storage.");
        if(newCapacity!=_capacity){
            T* ptr=nullptr;
            if(newCapacity>0){
                ptr=new T[newCapacity];
                if(preserveContent){
                    std::size_t newSize=_used<newCapacity?_used:newCapacity;
                    std::memcpy(ptr,_ptr,newSize*sizeof(T));
                }
            }

            delete [] _ptr;
            _ptr=ptr;
            _capacity=newCapacity;
            if(newCapacity<_used) _used=newCapacity;
        }
    }

    void assign(const T* buf,std::size_t sz){
        if(0==sz) return;
        if(sz>_capacity) resize(sz,false);
        std::memcpy(_ptr,buf,sz*sizeof(T));
        _used=sz;
    }

    void append(const T* buf,std::size_t sz){
        if(0==sz) return;
        resize(_used+sz,true);
        std::memcpy(_ptr+_used-sz,buf,sz*sizeof(T));
    }

    void append(T val){
        resize(_used+1,true);
        _ptr[_used-1]=val;
    }

    void append(const Buffer& buf){
        append(buf.begin(),buf.size());
    }

    std::size_t capacity()const{
        return _capacity;
    }

    std::size_t capacityBytes()const{
        return _capacity*sizeof(T);
    }

    void swap(Buffer& other)noexcept{
        std::swap(_ptr,other._ptr);
        std::swap(_capacity,other._capacity);
        std::swap(_used,other._used);
        std::swap(_ownMem,other._ownMem);
    }

    bool operator==(const Buffer& other)const{
        if(this!=&other){
            if(_used==other._used){
                if(_ptr&&other._ptr&&std::memcmp(_ptr,other._ptr,_used*sizeof(T))==0) return true;
                else return _used==0;
            }
            return false;
        }

        return true;
    }

    bool operator !=(const Buffer& other)const{
        return !(*this==other);
    }

    void clear(){
        std::memset(_ptr,0,_used*sizeof(T));
    }

    std::size_t size()const{
        return _used;
    }

    std::size_t sizeBytes()const{
        return _used*sizeof(T);
    }

    T* begin(){
        return _ptr;
    }
    const T* begin()const{
        return _ptr;
    }

    T* _end(){
        return _ptr+_used;
    }

    const T* end()const{
        return _ptr+_used;
    }

    bool empty()const{
        return 0==_used;
    }

    T& operator[](std::size_t index){
        static_assert(index<_used,"index<_used");
        return _ptr[index];
    }

    const T& operator[](std::size_t index)const{
        static_assert(index<_used,"index<_used");
        return _ptr[index];
    }
private:
    Buffer()=delete;

    std::size_t _capacity;
    std::size_t _used;
    T* _ptr;
    bool _ownMem;
};



}

#endif // BUFFER_H
