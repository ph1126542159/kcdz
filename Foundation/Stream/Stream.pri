win32{
    SOURCES +=    $$PWD/FileStream_WIN32.cpp
    HEADERS +=    $$PWD/FileStream_WIN32.h
}
unix{
    SOURCES +=    $$PWD/FileStream_POSIX.cpp
    HEADERS +=    $$PWD/FileStream_POSIX.h
}

DISTFILES +=

HEADERS += \
    $$PWD/BasicBufferedBidirectionalStreamBuf.h \
    $$PWD/Buffer.h \
    $$PWD/BufferAllocator.h \
    $$PWD/BufferedStreamBuf.h \
    $$PWD/DeflatingStreamBuf.h \
    $$PWD/FileStream.h \
    $$PWD/StreamCopier.h \
    $$PWD/StreamUtil.h

SOURCES += \
    $$PWD/DeflatingStreamBuf.cpp \
    $$PWD/FileStream.cpp \
    $$PWD/StreamCopier.cpp
