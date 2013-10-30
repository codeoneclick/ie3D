#-------------------------------------------------
#
# Project created by QtCreator 2013-10-28T00:03:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = indieEngineShader
TEMPLATE = app

QMAKE_CFLAGS += -mmacosx-version-min=10.8
QMAKE_CXXFLAGS += -mmacosx-version-min=10.8

SOURCES += main.cpp\
        indieEngineShader.cpp

HEADERS  += indieEngineShader.h

FORMS    += indieEngineShader.ui
