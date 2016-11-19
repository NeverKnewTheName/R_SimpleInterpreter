#include "simpleinterpreter.h"

#include "simplelexer.h"
#include "simpleparser.h"
#include "simpleast.h"

#include <QDebug>

SimpleInterpreter::SimpleInterpreter(SimpleParser *parser) :
    parser(parser)
{
    tree = parser->parse();
}

SimpleInterpreter::SimpleInterpreter(const QString &StringToInterpret, SymbolTable &GlobalSymbolTable)
{
    SimpleLexer *lexer = new SimpleLexer(StringToInterpret);
    parser = new SimpleParser(lexer,GlobalSymbolTable);
    tree = parser->parse();
}

SimpleInterpreter::~SimpleInterpreter()
{
    qDebug() << __PRETTY_FUNCTION__;
    if(tree != NULL)
        delete tree;
}

ValueNode *SimpleInterpreter::interpret()
{
    if(tree == NULL)
        return new ValueNode();

    return &tree->visit();
}
