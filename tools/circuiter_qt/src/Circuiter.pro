#-------------------------------------------------
#
# Project created by QtCreator 2017-07-07T12:46:11
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(printsupport): QT += printsupport
qtHaveModule(opengl): QT += opengl

CONFIG(debug, debug|release) {
    TARGET = circuiterd
} else {
    TARGET = circuiter
}

CONFIG += console
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES +=\
    model/Connect.cpp \
    model/Container.cpp \
    model/Element.cpp \
    model/Ground.cpp \
    model/Line.cpp \
    model/Nfet.cpp \
    model/Pfet.cpp \
    model/Pin.cpp \
    model/Power.cpp \
    model/Wire.cpp \
    gui/Frame.cpp \
    gui/GraphicsItem.cpp \
    gui/GraphicsScene.cpp \
    gui/GraphicsView.cpp \
    gui/MainWindow.cpp \
    Main.cpp \

HEADERS  += \
    model/Connect.h \
    model/Container.h \
    model/Element.h \
    model/Ground.h \
    model/Line.h \
    model/Nfet.h \
    model/Pfet.h \
    model/Pin.h \
    model/Power.h \
    model/Wire.h \
    gui/Frame.h \
    gui/GraphicsItem.h \
    gui/GraphicsScene.h \
    gui/GraphicsView.h \
    gui/MainWindow.h \

RESOURCES += \
    images.qrc

DESTDIR = ../output
