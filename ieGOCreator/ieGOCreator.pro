#-------------------------------------------------
#
# Project created by QtCreator 2014-01-15T23:49:33
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ieGOCreator
TEMPLATE = app

SOURCES += Sources/main.cpp\
           Sources/CMainWindow.cpp

HEADERS += Sources/CMainWindow.h

FORMS += Forms/CMainWindow.ui

unix: LIBS += -L$$PWD/../bin/ -lieCore-osx_d

INCLUDEPATH += $$PWD/../ieCore/Sources
DEPENDPATH += $$PWD/../ieCore/Sources

unix: PRE_TARGETDEPS += $$PWD/../bin/libieCore-osx_d.a

RESOURCES += \
    Resources/Bundle.qrc
