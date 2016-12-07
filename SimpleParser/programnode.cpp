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

ASTNode *ProgramNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *ProgramASTNode = new ASTNode(printNode(), parentNode);

    for( const std::unique_ptr<SimpleNode> &expr : ProgramExpressions)
    {
        expr->VisualizeNode(ProgramASTNode);
    }

    ProgramReturnStatement->VisualizeNode(ProgramASTNode);

    return ProgramASTNode;
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

std::unique_ptr<SimpleNode> ProgramNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ProgramNode(*this));
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > ProgramNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    //ToDO increase maxStackSize -> Enter/Exit program scope! stack build up/destruction!


    std::vector<std::unique_ptr<SimpleNode>>::const_iterator it = ProgramExpressions.begin();
    std::vector<std::unique_ptr<SimpleNode>>::const_iterator itEnd = ProgramExpressions.end();
    for( ;it != itEnd; ++it)
    {
        FlatAST = (*it)->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);
    }

    FlatAST = ProgramReturnStatement->FlatCompile(std::move(FlatAST), maxStackSize, CurrentPosition);

    FlatAST->emplace_back(deepCopy());
    CurrentPosition++;

    return std::move(FlatAST);
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
