#-------------------------------------------------
#
# Project created by QtCreator 2017-01-27T16:21:25
#
#-------------------------------------------------

DESTDIR = bin
QT       += testlib

QT       -= gui

TARGET = tst_grapetesttest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app


SOURCES += tst_grapetesttest.cpp \
    utility.cpp
DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    utility.h
