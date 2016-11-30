#include "simpleinterpreter.h"

#include "simplelexer.h"
#include "simpleparser.h"

#include "simplenode.h"
#include "valuenode.h"
#include "symboltable.h"

#include <QDebug>

SimpleInterpreter::SimpleInterpreter(SimpleParser *parser) :
    parser(parser)
{
    tree = parser->parse();
}

SimpleInterpreter::SimpleInterpreter(const QString &StringToInterpret, QSharedPointer<SymbolTable> GlobalSymbolTable)
{
    SimpleLexer *lexer = new SimpleLexer(StringToInterpret);
    parser = new SimpleParser(lexer,GlobalSymbolTable);
    tree = parser->parse();
}

SimpleInterpreter::~SimpleInterpreter()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::unique_ptr<ValueNode> SimpleInterpreter::interpret()
{
    if(tree == nullptr)
        return std::unique_ptr<ValueNode>();

    return tree->visit();
}
