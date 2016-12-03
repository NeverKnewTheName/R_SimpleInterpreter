#include "functioncallnode.h"

#include <QDebug>

#include "simplesymboltableentry.h"
#include "simplesymboltable.h"
#include "functionsymbol.h"
#include "simplestack.h"



FunctionCallNode::FunctionCallNode(
        const QString &FunctionName,
        QSharedPointer<SimpleSymbolTable> CurSymblTable,
        std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments
        ) :
    FunctionName(FunctionName),
    FuncArgs(std::move(FunctionArguments)),
    RelatedSymbol(qSharedPointerDynamicCast<FunctionSymbol>(CurSymblTable->lookup(FunctionName))),
    CurrentSymbolTable(CurSymblTable),
    returnType(RelatedSymbol->getReturnType())
{

}

FunctionCallNode::FunctionCallNode(const FunctionCallNode &ToCopy) :
    FunctionName(ToCopy.FunctionName),
    RelatedSymbol(ToCopy.RelatedSymbol),
    CurrentSymbolTable(ToCopy.CurrentSymbolTable),
    returnType(RelatedSymbol->getReturnType())
{
    for(const std::unique_ptr<SimpleNode> &arg : ToCopy.FuncArgs)
    {
        this->FuncArgs.emplace_back(arg->deepCopy());
    }
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

std::unique_ptr<ValueNode> FunctionCallNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    return RelatedSymbol->CallFunction(FuncArgs, StackToUse);
}

std::unique_ptr<SimpleNode> FunctionCallNode::deepCopy() const
{

}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FunctionCallNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const
{

}
