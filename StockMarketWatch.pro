#-------------------------------------------------
#
# Project created by QtCreator 2015-07-04T14:51:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

TARGET = StockMarketWatch
TEMPLATE = app

INCLUDEPATH += $$PWD/include

SOURCES += src/main.cpp\
        src/mainwindow.cpp \
    src/datafetcher.cpp \
    src/tablemodel.cpp \
    src/stockdata.cpp \
    lib/qcustomplot/qcustomplot.cpp \
    src/graphplotter.cpp \
    src/livestockwatchwindow.cpp \
    src/backgroundworker.cpp \
    src/backgrounddownloader.cpp

HEADERS  += include/mainwindow.h \
    include/datafetcher.h \
    include/tablemodel.h \
    include/stockdata.h \
    lib/qcustomplot/qcustomplot.h \
    include/graphplotter.h \
    include/livestockwatchwindow.h \
    include/backgroundworker.h \
    include/backgrounddownloader.h

FORMS    += forms/mainwindow.ui \
    forms/livestockwatchwindow.ui

unix|win32: LIBS += -L$$PWD/lib/curlcpp/ -lcurlcpp -lcurl

INCLUDEPATH += $$PWD/lib/curlcpp/include
DEPENDPATH += $$PWD/lib/curlcpp/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lib/curlcpp/curlcpp.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/lib/curlcpp/libcurlcpp.a

DISTFILES +=

RESOURCES += \
    resource.qrc

INCLUDEPATH += $$PWD/lib/rapidjson/
