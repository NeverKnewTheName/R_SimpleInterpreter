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
    simpleinterpreter.cpp \
    operationnodes.cpp \
    unaryoperationnodes.cpp \
    binaryoperationnodes.cpp \
    ternaryoperationnodes.cpp \
    variablenode.cpp \
    datanode.cpp \
    functioncallnode.cpp \
    assignmentnode.cpp \
    programnode.cpp \
    simplenode.cpp \
    valuenode.cpp \
    variablesymbol.cpp \
    functionsymbol.cpp \
    controlnode.cpp \
    simplesymbol.cpp \
    simplesymboltable.cpp \
    simplesymboltableentry.cpp \
    simplestack.cpp

HEADERS  += r_simpleinterpreter.h \
    simplelexer.h \
    simpletoken.h \
    simpleparser.h \
    simpleinterpreter.h \
    operationnodes.h \
    unaryoperationnodes.h \
    binaryoperationnodes.h \
    ternaryoperationnodes.h \
    variablenode.h \
    datanode.h \
    functioncallnode.h \
    assignmentnode.h \
    programnode.h \
    simplenode.h \
    valuenode.h \
    variablesymbol.h \
    functionsymbol.h \
    controlnode.h \
    simplesymbol.h \
    simplesymboltable.h \
    simplesymboltableentry.h \
    simplestack.h

FORMS    += r_simpleinterpreter.ui
