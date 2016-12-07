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

ASTNode *FunctionCallNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *FunctionASTNode = new ASTNode(printNode(), parentNode);

    const std::vector<QSharedPointer<VariableSymbol>> &params = RelatedSymbol->getFunctionParameters();
    std::vector<QSharedPointer<VariableSymbol>>::const_iterator itParams = params.begin();
    std::vector<QSharedPointer<VariableSymbol>>::const_iterator itParamsEnd = params.end();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itArgs = FuncArgs.begin();
    for(;itParams != itParamsEnd; ++itParams, ++itArgs)
    {
        ASTNode *paramAssignment = new ASTNode(QString("ParameterAssignment"),FunctionASTNode);
        ASTNode *parameterASTNode = new ASTNode(QString("Parameter: ").append((*itParams)->PrintToSymbolToString()), paramAssignment);
        new ASTNode((*itParams)->getIdentifier(), parameterASTNode);
        new ASTNode(QString("="), paramAssignment);
        (*itArgs)->VisualizeNode(paramAssignment);
    }

    const std::vector<std::unique_ptr<SimpleNode>> &FunctionExpressions = RelatedSymbol->getFunctionExpressions();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator it = FunctionExpressions.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itEnd = FunctionExpressions.end();
    for( ;it != itEnd; ++it)
    {
        (*it)->VisualizeNode(FunctionASTNode);
        new ASTNode(QString(";"), FunctionASTNode);
    }

    RelatedSymbol->getFunctionReturnNode()->VisualizeNode(FunctionASTNode);
    new ASTNode(QString(";"), FunctionASTNode);

    return FunctionASTNode;
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
//    return RelatedSymbol->CallFunction(FuncArgs, StackToUse);

    const std::vector<QSharedPointer<VariableSymbol> > &FunctionParameters = RelatedSymbol->getFunctionParameters();
    const std::vector<std::unique_ptr<SimpleNode>> &FunctionExpressions = RelatedSymbol->getFunctionExpressions();
    const std::unique_ptr<SimpleNode> &FunctionReturnNode = RelatedSymbol->getFunctionReturnNode();
    QSharedPointer<SimpleSymbolTable> FunctionSymbolTable = RelatedSymbol->getFunctionSymbolTable();
    if(FunctionParameters.size() != FuncArgs.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return std::unique_ptr<ValueNode>( new ValueNode());
    }

    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itFuncArgsBegin = FuncArgs.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itFuncArgsEnd = FuncArgs.end();
    std::vector<std::unique_ptr<ValueNode>> EvaluatedFuncArgs;
    for(; itFuncArgsBegin != itFuncArgsEnd; itFuncArgsBegin++)
    {
        EvaluatedFuncArgs.emplace_back(std::move((*itFuncArgsBegin)->visit(StackToUse)));
    }
    FunctionSymbolTable->EnterScope(StackToUse);

    const size_t NrOfParameters = FunctionParameters.size();
    for(size_t i = 0; i < NrOfParameters; i++)
    {
        const QSharedPointer<VariableSymbol> &param = FunctionParameters.at(i);
        std::unique_ptr<ValueNode> &argument = EvaluatedFuncArgs.at(i);
        param->assignValue(std::move(argument), StackToUse);
    }


    for(const std::unique_ptr<SimpleNode> &expression : FunctionExpressions)
    {
        expression->visit(StackToUse);
    }

    std::unique_ptr<ValueNode> returnValue = FunctionReturnNode->visit(StackToUse);

    FunctionSymbolTable->ExitScope(StackToUse);
    return returnValue;
}

std::unique_ptr<SimpleNode> FunctionCallNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new FunctionCallNode(*this));
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FunctionCallNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    //ToDO Increment maxStackSize! -> Scope!
    //Maybe add extra value node for scope stack size?? to enter/exit scope with stack build up/destruction...//ToTHINK

    const std::vector<std::unique_ptr<SimpleNode>> &FunctionExpressions = RelatedSymbol->getFunctionExpressions();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator it = FunctionExpressions.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itEnd = FunctionExpressions.end();
    for( ;it != itEnd; ++it)
    {
        FlatAST = (*it)->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);
    }

    FlatAST = RelatedSymbol->getFunctionReturnNode()->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);

    FlatAST->emplace_back(deepCopy());
    CurrentPosition++;

    return std::move(FlatAST);
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
