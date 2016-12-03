#include "simpleinterpreter.h"

#include "simplelexer.h"
#include "simpleparser.h"

#include "simplenode.h"
#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

#include <QDebug>

SimpleInterpreter::SimpleInterpreter(SimpleParser *parser) :
    parser(parser),
    InterpreterStack(100)
{
    tree = parser->parse();
}

SimpleInterpreter::SimpleInterpreter(
        const QString &StringToInterpret,
        QSharedPointer<SimpleSymbolTable> GlobalSymbolTable
        ) :
InterpreterStack(100)
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


    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatAST(new std::vector<std::unique_ptr<SimpleNode>>());
    int stackSize = 0;
    int CurrentPosition = 0;

    FlatAST = tree->FlatCompile(std::move(FlatAST), stackSize, CurrentPosition);

    const int flatASTSize = FlatAST->size();
    qDebug() << "FlatAST size: " << flatASTSize;
//    std::vector<std::unique_ptr<SimpleNode>> FlatASTVec = *FlatAST;
    for(int i = 0; i < flatASTSize; i++)
    {
        qDebug() << FlatAST->at(i)->printNode();
    }

    QSharedPointer<SimpleStack> RuntimeStack(new SimpleStack(100));
    return tree->visit(RuntimeStack);
}
