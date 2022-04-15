QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

INCLUDEPATH += \
    ../iVS3D-core/plugin \
    ../iVS3D-core/model

include(../../3rdparty.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

with_cuda{
DEFINES += WITH_CUDA
HEADERS += flowcalculatorcuda.h
HEADERS += imagegatherercuda.h
SOURCES += flowcalculatorcuda.cpp
SOURCES += imagegatherercuda.cpp
}

QT += concurrent

SOURCES += \
    factory.cpp \
    flowcalculator.cpp \
    flowcalculatorcpu.cpp \
    imagegatherer.cpp \
    imagegatherercpu.cpp \
    stationarycamera.cpp \

HEADERS += \
    factory.h \
    flowcalculator.h \
    flowcalculatorcpu.h \
    imagegatherer.h \
    imagegatherercpu.h \
    stationarycamera.h \

TEMPLATE = lib
CONFIG += plugin

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# create .dll in plugins folder
TARGET = $$qtLibraryTarget(stationarymovementplugin)
DESTDIR = ../plugins

FORMS +=
