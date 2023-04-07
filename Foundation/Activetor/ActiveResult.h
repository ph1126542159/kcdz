#ifndef ACTIVERESULT_H
#define ACTIVERESULT_H
#include <mutex>
#include <condition_variable>
#include <algorithm>
#include <assert.h>
#include "Core/RefCountedObject.h"
#include "Core/Exception.h"

namespace PH {
template<class ResultType>
class ActiveResultHolder:public RefCountedObject{
public:
    ActiveResultHolder():
        _pData(nullptr),
        _pExc(nullptr){

    }

    ResultType& data(){
        assert(_pData!=nullptr);
        return *_pData;
    }

    void data(ResultType* pData){
        delete _pData;
        _pData=pData;
    }

    void wait(){
        std::unique_lock<std::mutex> lck(_mtx);
        _event.wait(lck);
    }

    bool tryWait(long milliseconds){
        std::unique_lock<std::mutex> lck(_mtx);
        return _event.wait_for(lck,std::chrono::milliseconds(milliseconds))==std::cv_status::timeout;
    }

    void notify(){
        _event.notify_one();
    }

    bool failed()const{
        return nullptr!=_pExc;
    }

    std::string error()const{
        if(_pExc) return _pExc->displayText();
        return std::string();
    }

    void error(const PH::Exception& exc){
        delete _pExc;
        _pExc=exc.clone();
    }

    void error(const std::string& msg){
        delete _pExc;
        _pExc=new PH::Exception(msg);
    }

protected:
    ~ActiveResultHolder(){
        delete _pData;
        delete _pExc;
    }

private:
    ResultType* _pData;
    PH::Exception* _pExc;
    std::mutex _mtx;
    std::condition_variable _event;
};

template<>
class ActiveResultHolder<void>: public RefCountedObject{
public:
    ActiveResultHolder():
        _pExc(nullptr){

    }

    void wait(){
        std::unique_lock<std::mutex> lck(_mtx);
        _event.wait(lck);
    }

    bool tryWait(long milliseconds){
        std::unique_lock<std::mutex> lck(_mtx);
        return _event.wait_for(lck,std::chrono::milliseconds(milliseconds))==std::cv_status::timeout;
    }

    void notify(){
        _event.notify_one();
    }

    bool failed()const{
        return nullptr!=_pExc;
    }

    std::string error()const{
        if(_pExc) return _pExc->displayText();
        return std::string();
    }

    PH::Exception* exception()const{
        return _pExc;
    }

    void error(const PH::Exception& exc){
        delete _pExc;
        _pExc=exc.clone();
    }

    void error(const std::string& msg){
        delete _pExc;
        _pExc=new PH::Exception(msg);
    }


protected:
    ~ActiveResultHolder(){
        delete _pExc;
    }
private:
    PH::Exception* _pExc;
    std::condition_variable _event;
    std::mutex _mtx;
};

template<class RT>
class ActiveResult{
public:
    typedef RT ResultType;
    typedef ActiveResultHolder<ResultType> ActiveResultHolderType;

    ActiveResult(ActiveResultHolderType* pHolder):
        _pHolder(pHolder){

    }

    ActiveResult(const ActiveResult& result){
        _pHolder=result._pHolder;
        _pHolder->duplicate();
    }

    ~ActiveResult(){
        _pHolder->release();
    }

    ActiveResult& operator =(const ActiveResult& result){
        ActiveResult tmp(result);
        swap(tmp);
        return *this;
    }

    void swap(ActiveResult& result){
        std::swap(_pHolder,result._pHolder);
    }

    ResultType& data()const{
        return _pHolder->data();
    }

    void data(ResultType* pValue){
        _pHolder->data(pValue);
    }

    void wait(){
        _pHolder->wait();
    }

    bool tryWait(long milliseconds){
        return _pHolder->tryWait(milliseconds);
    }

    bool avaliable()const{
        return _pHolder->tryWait(0);
    }

    bool failed()const{
        return _pHolder->failed();
    }

    std::string error()const{
        return _pHolder->error();
    }

    PH::Exception* exception()const{
        return _pHolder->exception();
    }

    void notify(){
        _pHolder->notify();
    }

    ResultType& data(){
        return _pHolder->data();
    }

    void error(const std::string& msg){
        _pHolder->error(msg);
    }

    void error(const PH::Exception& exc){
        _pHolder->error(exc);
    }

private:
    ActiveResult()=delete;

    ActiveResultHolderType* _pHolder;
};


template<>
class ActiveResult<void>{
public:
    typedef ActiveResultHolder<void> ActiveResultHolderType;

    ActiveResult(ActiveResultHolderType* pHolder):
        _pHolder(pHolder){
        assert(nullptr!=pHolder);
    }

    ActiveResult(const ActiveResult& result){
        _pHolder=result._pHolder;
        _pHolder->duplicate();
    }

    ~ActiveResult(){
        _pHolder->release();
    }

    ActiveResult& operator =(const ActiveResult& result){
        ActiveResult tmp(result);
        swap(tmp);
        return *this;
    }

    void swap(ActiveResult& other){
        std::swap(_pHolder,other._pHolder);
    }

    void wait(){
        _pHolder->wait();
    }

    bool tryWait(long milliseconds){
        return _pHolder->tryWait(milliseconds);
    }

    bool avaliable()const{
        return _pHolder->tryWait(0);
    }

    bool failed()const{
        return _pHolder->failed();
    }

    std::string error()const{
        return _pHolder->error();
    }

    Exception* exception()const{
        return _pHolder->exception();
    }

    void notify(){
        _pHolder->notify();
    }

    void error(const std::string& msg){
        _pHolder->error(msg);
    }

    void error(const PH::Exception& exc){
        _pHolder->error(exc);
    }
private:
    ActiveResult()=delete;

    ActiveResultHolderType* _pHolder;
};

}

#endif // ACTIVERESULT_H
