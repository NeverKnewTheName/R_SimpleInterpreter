#ifndef SIMPLEINTERPRETER_H
#define SIMPLEINTERPRETER_H

#include <QString>
#include "simplesymboltable.h"

class SimpleNode;
class ValueNode;
class SimpleParser;

class SimpleInterpreter
{
public:
    SimpleInterpreter(SimpleParser *parser);
    SimpleInterpreter(const QString &StringToInterpret, SymbolTable &GlobalSymbolTable);
    ~SimpleInterpreter();

    ValueNode *interpret();
private:
    SimpleParser *parser;
    SimpleNode *tree;
};

#endif // SIMPLEINTERPRETER_H
