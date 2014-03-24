#-------------------------------------------------
#
# Project created by QtCreator 2014-02-14T12:28:46
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutomatyKomorkowe
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    PopulationViewWidget.cpp \
    GridSettingsWindow.cpp \
    CellInfo.cpp \
    CellularAutomaton.cpp

HEADERS  += MainWindow.hpp \
    PopulationViewWidget.hpp \
    GridSettingsWindow.hpp \
    CellInfo.hpp \
    CellularAutomaton.hpp

RESOURCES += \
    Icons.qrc
