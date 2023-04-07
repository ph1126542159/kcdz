#ifndef REDISBUGCHECK_H
#define REDISBUGCHECK_H
#include <exception>
#include <string>
#include <assert.h>

namespace Redis {
class Exception : public std::exception{
public:
    Exception(const char* str,int code=0):
        _str(str),_code(code){

    }

    Exception(const std::string& str,int code=0):
        _str(str),_code(code){

    }

    std::string displayText()const{return _str;}

    int errCode()const{return _code;}

private:
    std::string _str;

    int _code;
};
}

#endif // REDISBUGCHECK_H
