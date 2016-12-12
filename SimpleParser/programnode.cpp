#include "programnode.h"

#include <QDebug>

#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "functionsymbol.h"
#include "simplestack.h"

#include "astvisualizer.h"
#include "simplenodevisitor.h"

ProgramNode::ProgramNode(const QString &ProgramName,
        QSharedPointer<SimpleSymbolTable> SymbolTableToUse
        ) :
    ProgramName(ProgramName),
    ProgramSymbolTable(SymbolTableToUse)
{

}

ProgramNode::ProgramNode(const ProgramNode &ToCopy) :
    ProgramName(ToCopy.ProgramName),
    ProgramSymbolTable(ToCopy.ProgramSymbolTable),
    ProgramReturnStatement(ToCopy.ProgramReturnStatement->deepCopy())
{
    for(const std::unique_ptr<SimpleNode> &expr : ToCopy.ProgramExpressions)
    {
        this->ProgramExpressions.emplace_back(expr->deepCopy());
    }
}

ProgramNode::~ProgramNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void ProgramNode::addExpression(std::unique_ptr<SimpleNode> &newExpression)
{
    ProgramExpressions.emplace_back(std::move(newExpression));
}

void ProgramNode::addFunctionDefinition(QSharedPointer<FunctionSymbol> newFunction)
{
    ProgramSymbolTable->addEntry(newFunction);
}

void ProgramNode::addVariableDefinition(QSharedPointer<VariableSymbol> newVariable)
{
    ProgramSymbolTable->addEntry(newVariable);
}

void ProgramNode::addReturnStatement(std::unique_ptr<SimpleNode> ReturnStatement)
{
    //Maybe std::move??
    ProgramReturnStatement.reset(ReturnStatement.release());
}

Node::NodeType ProgramNode::getNodeType() const
{
    return Node::Program;
}

Node::ValueTypes ProgramNode::getReturnType() const
{
    return type;
}

QString ProgramNode::printValue() const
{
    return ProgramName;
}

QString ProgramNode::printNode() const
{
    return QString("{ProgramNode}:{%1}").arg(printValue());
}

std::unique_ptr<SimpleNode> ProgramNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ProgramNode(*this));
}

const std::unique_ptr<SimpleNode> &ProgramNode::getProgramReturnStatement() const
{
    return ProgramReturnStatement;
}

const std::vector<std::unique_ptr<SimpleNode> > &ProgramNode::getProgramExpressions() const
{
    return ProgramExpressions;
}

QSharedPointer<SimpleSymbolTable> ProgramNode::getProgramSymbolTable() const
{
    return ProgramSymbolTable;
}


void ProgramNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ProgramNode>(new ProgramNode(*this)));
}
