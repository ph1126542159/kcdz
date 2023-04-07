﻿#include "FileStream.h"
#include "Core/Exception.h"


namespace PH {

FileIOS::FileIOS(std::ios::openmode defaultMode):
    _defaultMode(defaultMode){
    ph_ios_init(&_buf);
}

FileIOS::~FileIOS(){

}


void FileIOS::open(const std::string &path, std::ios::openmode mode){
    clear();
    _buf.open(path,mode|_defaultMode);
}

void FileIOS::close(){
    if(!_buf.close()){
        setstate(ios_base::badbit);
    }
}

FileStreamBuf* FileIOS::rdbuf(){
    return &_buf;
}

FileInputStream::FileInputStream():
    FileIOS(std::ios::in),
    std::istream(&_buf){

}

FileInputStream::FileInputStream(const std::string &path, std::ios::openmode mode):
    FileIOS(std::ios::in),
    std::istream(&_buf){
    open(path,mode);
}

FileInputStream::~FileInputStream(){

}

FileOutputStream::FileOutputStream():
    FileIOS(std::ios::out),
    std::ostream(&_buf){

}

FileOutputStream::FileOutputStream(const std::string &path, std::ios::openmode mode):
    FileIOS(std::ios::out),
    std::ostream(&_buf){
    open(path,mode);
}

FileOutputStream::~FileOutputStream(){

}

FileStream::FileStream():
    FileIOS(std::ios::in|std::ios::out),
    std::iostream(&_buf){

}

FileStream::FileStream(const std::string &path, std::ios::openmode mode):
    FileIOS(std::ios::in|std::ios::out),
    std::iostream(&_buf){
    open(path,mode);
}

FileStream::~FileStream(){

}

}
