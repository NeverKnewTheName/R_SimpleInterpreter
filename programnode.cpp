#include "programnode.h"

#include <QDebug>

#include "symboltable.h"
#include "variablesymbol.h"
#include "functionsymbol.h"

ProgramNode::ProgramNode(const QString &ProgramName,
        QSharedPointer<SymbolTable> SymbolTableToUse
        ) :
    ProgramName(ProgramName),
    ProgramSymbolTable(SymbolTableToUse)
{

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
    ProgramSymbolTable->addEntry(newFunction->getIdentifier(), newFunction);
}

void ProgramNode::addVariableDefinition(QSharedPointer<VariableSymbol> newVariable)
{
    ProgramSymbolTable->addEntry(newVariable->getIdentifier(), newVariable);
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

std::unique_ptr<ValueNode> ProgramNode::visit() const
{
    const int NrOfProgramExpression = ProgramExpressions.size();

    for(int i = 0; i < NrOfProgramExpression; i++)
    {
        ProgramExpressions.at(i)->visit();
    }

    return ProgramReturnStatement->visit();
}
