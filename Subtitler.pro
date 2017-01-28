#-------------------------------------------------
#
# Project created by QtCreator 2017-01-06T13:28:50
#
#-------------------------------------------------

QT       += core gui widgets
TEMPLATE = app
CONFIG += c++14 precompile_header
TARGET = Subtitler
QMAKE_MAC_SDK = macosx10.12
QMAKE_MACOSX_DEPLOYMENT_TARGET = macosx10.11
ICON = Resources\subtitles-icon.icns

DESTDIR = ./bin
OBJECTS_DIR = ./obj
MOC_DIR = ./obj/GeneratedFiles
INCLUDEPATH += .
UI_DIR = ./GeneratedFiles
RCC_DIR = ./GeneratedFiles


SOURCES += main.cpp\
	MainWindow.cpp \
    DetailedMessageBox.cpp \
    TabPageWidget.cpp \
    LoadSrtThread.cpp \
    FormatThread.cpp

HEADERS  += MainWindow.h \
    stdafx.h \
    Exception.h \
    DetailedMessageBox.h \
    TabPageWidget.h \
    LoadSrtThread.h \
    SrtItem.h \
    FormatThread.h

FORMS    += MainWindow.ui \
    TabPageWidget.ui


PRECOMPILED_HEADER = stdafx.h

RESOURCES += \
    res.qrc

TRANSLATIONS += subtitler_ru.ts
