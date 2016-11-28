#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T18:04:51
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = R_SimpleInterpreter
TEMPLATE = app


SOURCES += main.cpp\
        r_simpleinterpreter.cpp \
    simplelexer.cpp \
    simpletoken.cpp \
    simpleparser.cpp \
    simpleast.cpp \
    simplesymboltable.cpp \
    simpleinterpreter.cpp \
    operationnodes.cpp \
    unaryoperationnodes.cpp \
    binaryoperationnodes.cpp \
    ternaryoperationnodes.cpp \
    symbolnodes.cpp

HEADERS  += r_simpleinterpreter.h \
    simplelexer.h \
    simpletoken.h \
    simpleparser.h \
    simpleast.h \
    simplesymboltable.h \
    simpleinterpreter.h \
    operationnodes.h \
    unaryoperationnodes.h \
    binaryoperationnodes.h \
    ternaryoperationnodes.h \
    symbolnodes.h

FORMS    += r_simpleinterpreter.ui
