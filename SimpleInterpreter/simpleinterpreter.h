#ifndef SIMPLEINTERPRETER_H
#define SIMPLEINTERPRETER_H

#include <QObject>
#include <QString>
#include <QSharedPointer>
#include <memory>

#include "simplestack.h"

class SimpleNode;
class ValueNode;
class SimpleParser;
class SimpleSymbolTable;

class ASTNode;

class SimpleInterpreter : QObject
{
    Q_OBJECT
public:
    SimpleInterpreter(SimpleParser *parser);
    SimpleInterpreter(const QString &StringToInterpret, QSharedPointer<SimpleSymbolTable> GlobalSymbolTable);
    ~SimpleInterpreter();

    ASTNode *VisualizeAST();
    std::unique_ptr<ValueNode> interpret();
private:
    SimpleParser *parser;
    std::unique_ptr<SimpleNode> tree;
    SimpleStack InterpreterStack;
};

#endif // SIMPLEINTERPRETER_H
