QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = client.out
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
        main.cpp \
        main_window.cpp \
    client.cpp \
    json_highlighter.cpp

HEADERS += \
        main_window.h \
    client.h \
    json_highlighter.h

FORMS += \
        main_window.ui
