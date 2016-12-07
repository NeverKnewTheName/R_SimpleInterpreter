#-------------------------------------------------
#
# Project created by QtCreator 2016-11-10T18:04:51
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = R_SimpleInterpreter
TEMPLATE = app


SOURCES += \
    main.cpp\
    r_simpleinterpreter.cpp \
    astvisualizer.cpp

HEADERS  += \
    r_simpleinterpreter.h \
    astvisualizer.h

FORMS    += \
    r_simpleinterpreter.ui



include(SimpleLexer/SimpleLexer.pri)
include(SimpleParser/SimpleParser.pri)
include(SimpleInterpreter/SimpleInterpreter.pri)
include(SimpleSymbolTable/SimpleSymbolTable.pri)
include(SimpleStack/SimpleStack.pri)
include(SimpleFlatCompiler/SimpleFlatCompiler.pri)
include(SimpleNodeVisitor/SimpleNodeVisitor.pri)
