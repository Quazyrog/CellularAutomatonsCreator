#-------------------------------------------------
#
# Project created by QtCreator 2014-02-14T12:28:46
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CellularAutomatonCreator
TEMPLATE = app


SOURCES += main.cpp \
    MainWindow.cpp \
    CellularAutomaton.cpp \
    Exception.cpp \
    GridViewer.cpp \
    StateSwitchWindow.cpp \
    Calculator.cpp \
    FunctionOffset.cpp \
    FunctionStat.cpp

HEADERS  += \
    MainWindow.hpp \
    CellularAutomaton.hpp \
    Exception.hpp \
    GridViewer.hpp \
    StateSwitchWindow.hpp \
    Calculator.hpp \
    FunctionOffset.hpp \
    FunctionStat.hpp

RESOURCES += \
    Images.qrc
