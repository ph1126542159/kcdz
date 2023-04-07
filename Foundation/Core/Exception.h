#ifndef PH_EXCEPTION_H
#define PH_EXCEPTION_H
#include <stdexcept>
#include <string>

namespace PH {
class Exception : public std::exception{
public:
    Exception(const std::string& msg, int code = 0);

    Exception(const std::string& msg, const std::string& arg, int code = 0);

    Exception(const std::string& msg, const Exception& nested, int code = 0);

    Exception(const Exception& exc);

    ~Exception()noexcept;

    Exception& operator =(const Exception& exc);

    virtual const char* name()const noexcept;

    virtual const char* className()const noexcept;

    virtual const char* what() const noexcept;

    const Exception* nested()const;

    const std::string& message()const;

    int code()const;

    std::string displayText()const;

    virtual Exception* clone()const;

    virtual void rethrow()const;

protected:
    explicit Exception(int code=0);

    void message(const std::string& msg);

    void extendedMessage(const std::string& arg);
private:
    std::string _msg;
    Exception* _pNested;
    int _code;
};


inline const Exception* Exception::nested()const {
    return _pNested;
}

inline const std::string& Exception::message()const {
    return _msg;
}

inline void Exception::message(const std::string& msg) {
    _msg = msg;
}

inline int Exception::code()const {
    return _code;
}

#define PH_DECLARE_EXCEPYION_CODE(CLS,BASE,CODE) \
    class CLS : public BASE {\
        public:\
            CLS(int code=CODE);\
            CLS(const std::string& msg,int code=CODE);\
            CLS(const std::string& msg,const std::string& arg,int code=CODE);\
            CLS(const std::string& msg, const Exception& exc, int code = CODE);\
            CLS(const CLS& exc);\
            ~CLS()noexcept;\
            CLS& operator=(const CLS& exc);\
            const char* name()const noexcept;\
            const char* className()const noexcept;\
            Exception* clone()const;\
            void rethrow()const;\
    };


#define PH_DECLARE_EXCEPTION(CLS,BASE)\
    PH_DECLARE_EXCEPYION_CODE(CLS,BASE,0)

#define PH_IMPLEMENT_EXCEPTION(CLS,BASE,NAME)\
    CLS::CLS(int code):BASE(code){\
    }\
    CLS::CLS(const std::string& msg, int code): BASE(msg, code){\
    }\
    CLS::CLS(const std::string& msg,const std::string& arg,int code):BASE(msg,arg,code){\
    }\
    CLS::CLS(const std::string& msg ,const Exception& exc,int code):BASE(msg,exc ,code){\
    }\
    CLS::CLS(const CLS& exc):BASE(exc){\
    }\
    CLS::~CLS()noexcept {\
    }\
    CLS& CLS::operator=(const CLS &exc){\
        BASE::operator=(exc);\
        return *this;\
    }\
    const char* CLS::name()const noexcept {\
        return NAME;\
    }\
    const char* CLS::className()const noexcept{\
        return typeid(*this).name();\
    }\
    Exception* CLS::clone()const{\
        return new CLS(*this);\
    }\
    void CLS::rethrow()const{\
        throw *this;\
    }


    PH_DECLARE_EXCEPTION(InvalidArgumentException,Exception)
    PH_DECLARE_EXCEPTION(IOException,Exception)
    PH_DECLARE_EXCEPTION(NotImplementedException,Exception)
    PH_DECLARE_EXCEPTION(InvalidAccessException,Exception)
    PH_DECLARE_EXCEPTION(IllegalStateException,Exception)
    PH_DECLARE_EXCEPTION(NullPointerException,Exception)

}

#endif // EXCEPTION_H
