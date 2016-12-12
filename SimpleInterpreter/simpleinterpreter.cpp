#include "simpleinterpreter.h"

#include "simplelexer.h"
#include "simpleparser.h"

#include "simplenode.h"
#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

#include "astvisualizer.h"

#include <QDebug>

SimpleInterpreter::SimpleInterpreter(SimpleParser *parser) :
    parser(parser),
    InterpreterStack(100)
{
    tree = parser->ParseToAST();
}

SimpleInterpreter::SimpleInterpreter(
        const QString &StringToInterpret,
        QSharedPointer<SimpleSymbolTable> GlobalSymbolTable
        ) :
InterpreterStack(100)
{
    SimpleLexer *lexer = new SimpleLexer(StringToInterpret);
    parser = new SimpleParser(lexer,GlobalSymbolTable);
    tree = parser->ParseToAST();
}

SimpleInterpreter::~SimpleInterpreter()
{
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

ASTNode *SimpleInterpreter::VisualizeAST()
{
    if(tree == nullptr)
        return new ASTNode(QString("ERROR"));

//    return tree->VisualizeNode();
}

std::unique_ptr<ValueNode> SimpleInterpreter::interpret()
{
    if(tree == nullptr)
        return std::unique_ptr<ValueNode>();

    QSharedPointer<SimpleStack> RuntimeStack(new SimpleStack(100));
//    return tree->visit(RuntimeStack);
}
