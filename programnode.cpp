#include "programnode.h"

#include <QDebug>

#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "functionsymbol.h"
#include "simplestack.h"

ProgramNode::ProgramNode(const QString &ProgramName,
        QSharedPointer<SimpleSymbolTable> SymbolTableToUse
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

bool ProgramNode::BuildProgramStack(QSharedPointer<SimpleStack> StackToUse) const
{
    bool IsSuccessfull = true;
    const std::vector<QSharedPointer<SimpleSymbolTableEntry>> SymbolTableEntries = ProgramSymbolTable->getSymbolTableEntries();

    for(auto & symbol: SymbolTableEntries)
    {
        if(symbol->getType() == SimpleSymbolTableEntry::Variable)
        {
            IsSuccessfull = IsSuccessfull || qSharedPointerDynamicCast<VariableSymbol>(symbol)->VarEnterScope(StackToUse);
        }
    }

    return IsSuccessfull;
}

bool ProgramNode::DestroyProgramStack(QSharedPointer<SimpleStack> StackToUse) const
{
    bool IsSuccessfull = true;
    const std::vector<QSharedPointer<SimpleSymbolTableEntry>> SymbolTableEntries = ProgramSymbolTable->getSymbolTableEntries();

    for(auto & symbol: SymbolTableEntries)
    {
        if(symbol->getType() == SimpleSymbolTableEntry::Variable)
        {
            IsSuccessfull = IsSuccessfull || qSharedPointerDynamicCast<VariableSymbol>(symbol)->VarEnterScope(StackToUse);
        }
    }

    return IsSuccessfull;
}


std::unique_ptr<ValueNode> ProgramNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    ProgramSymbolTable->EnterScope(StackToUse);
    const int NrOfProgramExpression = ProgramExpressions.size();

    for(int i = 0; i < NrOfProgramExpression; i++)
    {
        ProgramExpressions.at(i)->visit(StackToUse);
    }

    std::unique_ptr<ValueNode> result = ProgramReturnStatement->visit(StackToUse);

    ProgramSymbolTable->ExitScope(StackToUse);
    return result;
}
