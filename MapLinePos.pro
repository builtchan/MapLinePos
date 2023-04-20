#-------------------------------------------------
#
# Project created by QtCreator 2017-02-08T14:39:00
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MapLinePos
TEMPLATE = app


SOURCES += main.cpp\
        maplinepos.cpp \
    xmlparse.cpp

HEADERS  += maplinepos.h \
    CMyLabel.h \
    CRectLabel.h \
    xmlparse.h
