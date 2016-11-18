#ifndef SIMPLEINTERPRETER_H
#define SIMPLEINTERPRETER_H

#include <QString>

class SimpleNode;
class ValueNode;
class SimpleParser;

class SimpleInterpreter
{
public:
    SimpleInterpreter(SimpleParser *parser);
    SimpleInterpreter(const QString &StringToInterpret);
    ~SimpleInterpreter();

    ValueNode *interpret();
private:
    SimpleParser *parser;
    SimpleNode *tree;
};

#endif // SIMPLEINTERPRETER_H
