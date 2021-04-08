#-------------------------------------------------
#
# Project created by QtCreator 2018-08-07T11:38:24
#
#-------------------------------------------------

CONFIG += c++17

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IPUsing
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        ipusing.cpp \
    iplist.cpp \
    iplistdata.cpp \
    iplistmodel.cpp \
    ipnetwork.cpp \
    ipaddress.cpp \
    editfreenetwork.cpp \
    settings.cpp \
    transaction.cpp \
    editbusynetwork.cpp \
    editsizenetwork.cpp

HEADERS += \
        ipusing.h \
    iplist.h \
    iplistdata.h \
    iplistmodel.h \
    ipnetwork.h \
    ipaddress.h \
    editfreenetwork.h \
    settings.h \
    transaction.h \
    editbusynetwork.h \
    editsizenetwork.h

FORMS += \
        ipusing.ui \
    editfreenetwork.ui \
    editbusynetwork.ui \
    editsizenetwork.ui \
    settings.ui
