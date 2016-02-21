#-------------------------------------------------
#
# Project created by QtCreator 2016-02-16T06:39:11
#
#-------------------------------------------------

TARGET = qttest
TEMPLATE = app
CONFIG+= c++11\
                    console
CONFIG-=qt
CONFIG -= app_bundle

SOURCES += main.cpp \
    spcode.cpp
#---------------------------
#
#-----------------------------
LIBS +=/usr/lib/x86_64-linux-gnu/libboost*\
             /usr/lib/libgsl*

HEADERS += \
    spcode.h \
    spcrand.hpp

