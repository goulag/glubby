#-------------------------------------------------
#
# Project created by QtCreator 2014-10-14T11:23:17
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = glubby
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

LIBS += -L/usr/local/lib -lOGDF -lCOIN
INCLUDEPATH += ./src

OBJECTS_DIR = .o
MOC_DIR = .moc

SOURCES += src/main.cpp \
    src/glubby.cpp \
    src/baselayout.cpp \
    src/layouts/fmmmlayout.cpp \
    src/layouts/sugiyamalayout.cpp \
    src/layouts/circularlayout.cpp \
    src/layouts/gemlayout.cpp

HEADERS += \
    src/glubby.h \
    src/baselayout.h \
    src/layouts/fmmmlayout.h \
    src/layouts/sugiyamalayout.h \
    src/layouts/circularlayout.h \
    src/layouts/gemlayout.h
