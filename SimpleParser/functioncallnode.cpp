#include "functioncallnode.h"

#include <QDebug>

#include "assignmentnode.h"

#include "simplesymboltableentry.h"
#include "simplesymboltable.h"
#include "functionsymbol.h"
#include "simplestack.h"

#include "astvisualizer.h"
#include "simplenodevisitor.h"

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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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


std::unique_ptr<SimpleNode> FunctionCallNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new FunctionCallNode(*this));
}

QSharedPointer<FunctionSymbol> FunctionCallNode::getRelatedSymbol() const
{
    return RelatedSymbol;
}

const std::vector<std::unique_ptr<SimpleNode> > &FunctionCallNode::getFuncArgs() const
{
    return FuncArgs;
}

QSharedPointer<SimpleSymbolTable> FunctionCallNode::getCurrentSymbolTable() const
{
    return CurrentSymbolTable;
}


void FunctionCallNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<FunctionCallNode>(new FunctionCallNode(*this)));
}
