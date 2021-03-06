#-------------------------------------------------
#
# Project created by QtCreator 2017-09-13T23:58:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += datavisualization

TARGET = Poisson
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

INCLUDEPATH += $$quote("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.0/include")

LIBS += -L$$quote("C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v9.0/lib/x64") -lOpenCL

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    solvecontroller.cpp \
    plot3d.cpp \
    gridvalues.cpp \
    residual.cpp \
    iterators.cpp \
    cljacobiiterator.cpp

HEADERS += \
        mainwindow.h \
    solvecontroller.h \
    plot3d.h \
    gridvalues.h \
    residual.h \
    functions.h \
    iterators.h \
    cljacobiiterator.h

FORMS += \
        mainwindow.ui

DISTFILES += \
    JacobiKernel.cl
