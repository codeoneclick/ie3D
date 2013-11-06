#-------------------------------------------------
#
# Project created by QtCreator 2013-11-06T15:15:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = indieEngineShader
TEMPLATE = app


SOURCES += main.cpp\
        CMainWindowGUI.cpp \
    CShaderCompileGUI.cpp \
    CMaterialSettingsGUI.cpp \
    CMaterialExportGUI.cpp

HEADERS  += CMainWindowGUI.h \
    CShaderCompileGUI.h \
    CMaterialSettingsGUI.h \
    CMaterialExportGUI.h

FORMS    += CMainWindowGUI.ui \
    CShaderCompileGUI.ui \
    CMaterialSettingsGUI.ui \
    CMaterialExportGUI.ui
