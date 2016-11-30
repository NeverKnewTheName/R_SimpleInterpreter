#ifndef SIMPLEINTERPRETER_H
#define SIMPLEINTERPRETER_H

#include <QString>
#include <QSharedPointer>
#include <memory>

class SimpleNode;
class ValueNode;
class SimpleParser;
class SymbolTable;

class SimpleInterpreter
{
public:
    SimpleInterpreter(SimpleParser *parser);
    SimpleInterpreter(const QString &StringToInterpret, QSharedPointer<SymbolTable> GlobalSymbolTable);
    ~SimpleInterpreter();

    std::unique_ptr<ValueNode> interpret();
private:
    SimpleParser *parser;
    std::unique_ptr<SimpleNode> tree;
};

#endif // SIMPLEINTERPRETER_H
