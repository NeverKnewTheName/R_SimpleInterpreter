#include "functioncallnode.h"

#include <QDebug>

#include "symboltableentry.h"
#include "symboltable.h"
#include "functionsymbol.h"



FunctionCallNode::FunctionCallNode(
        const QString &FunctionName,
        QSharedPointer<SymbolTable> CurSymblTable,
        std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments
        ) :
    FunctionName(FunctionName),
    FuncArgs(std::move(FunctionArguments)),
    RelatedSymbol(qSharedPointerDynamicCast<FunctionSymbol>(CurSymblTable->lookup(FunctionName))),
    CurrentSymbolTable(CurSymblTable),
    returnType(RelatedSymbol->getReturnType())
{

}

FunctionCallNode::~FunctionCallNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType FunctionCallNode::getNodeType() const
{
    return Node::FunctionCall;
}

Node::ValueTypes FunctionCallNode::getReturnType() const
{
    return returnType;
}

QString FunctionCallNode::printValue() const
{
    return QString("%1()").arg(FunctionName);
}

QString FunctionCallNode::printNode() const
{
    return QString("{FunctionCall}:{%1}").arg(printValue());
}

std::unique_ptr<ValueNode> FunctionCallNode::visit() const
{
    return RelatedSymbol->CallFunction(FuncArgs, CurrentSymbolTable);
}
