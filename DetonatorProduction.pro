QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17 no_batch

#DEFINES += ENABLE_XLSS
if(contains(DEFINES, ENABLE_XLSS)){
    QT += xlsx
}

SOURCES += \
    DBManager.cpp \
    GlobalSetting.cpp \
    UI/LogWidget.cpp \
    UI/MainWindow.cpp \
    UI/ProduceDetInfo.cpp \
    UI/ProductionSet.cpp \
    UI/SerialPortSet.cpp \
    UI/TaskMgr.cpp \
    main.cpp \
    model/DET_PRODUCE.cpp \
    model/productDetData.cpp

HEADERS += \
    DBManager.h \
    GlobalSetting.h \
    TableOptional.h \
    UI/LogWidget.h \
    UI/MainWindow.h \
    UI/ProduceDetInfo.h \
    UI/ProductionSet.h \
    UI/SerialPortSet.h \
    UI/TaskMgr.h \
    model/DET_PRODUCE.h \
    model/productDetData.h

FORMS += \
    UI/LogWidget.ui \
    UI/MainWindow.ui \
    UI/ProduceDetInfo.ui \
    UI/ProductionSet.ui \
    UI/SerialPortSet.ui \
    UI/TaskMgr.ui

MOC_DIR         = temp/moc
RCC_DIR         = temp/rcc
UI_DIR          = temp/ui
OBJECTS_DIR     = temp/obj

CONFIG(release, debug|release): DESTDIR =$$PWD/OutPut/release
else:CONFIG(debug, debug|release): DESTDIR =$$PWD/OutPut/debug


include($$PWD/SerialPortHelper/SerialPortHelper.pri)
include($$PWD/Network/Network.pri)
include($$PWD/Gui/Gui.pri)

INCLUDEPATH+=$$PWD/Foundation/ \
             $$PWD/UI/
include($$PWD/Foundation/Core/Core.pri)
include($$PWD/Foundation/Logger/Logger.pri)
include($$PWD/Foundation/Notification/Notification.pri)
include($$PWD/Foundation/Stream/Stream.pri)
include($$PWD/Foundation/zlib/zlib.pri)
include($$PWD/Foundation/Activetor/Activetor.pri)




unix{
}

win32{
}
