#include "functionsymbol.h"

#include <QDebug>

#include "symboltable.h"
#include "variablesymbol.h"

FunctionSymbol::FunctionSymbol(
        const QString &identifier,
        QSharedPointer<SymbolTable> SymbolTableForFunction,
        std::vector<QSharedPointer<VariableSymbol> > &&functionParameters,
        Node::ValueTypes ReturnType
        ) :
    Symbol(identifier),
    FunctionSymbolTable(SymbolTableForFunction),
    ReturnType(ReturnType),
    FunctionParameters(functionParameters),
    FunctionReturnNode(new ValueNode())
{
    const int NrOfFunctionParameters = FunctionParameters.size();
    for(int i = 0; i < NrOfFunctionParameters; i++)
    {
        QString paramIdentifier = FunctionParameters.at(i)->getIdentifier();
        FunctionSymbolTable->addEntry(paramIdentifier,qSharedPointerDynamicCast<SymbolTableEntry>(FunctionParameters.at(i)));
    }
}

FunctionSymbol::~FunctionSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void FunctionSymbol::addFunctionExpressions(std::vector<std::unique_ptr<SimpleNode> > &FuncExpressions)
{
    FunctionExpressions = std::move(FuncExpressions);
}

void FunctionSymbol::addFunctionReturnStatement(std::unique_ptr<SimpleNode> returnNode)
{
    FunctionReturnNode = std::move(returnNode);
}

void FunctionSymbol::addVariableDefinition(QSharedPointer<VariableSymbol> newVariable)
{
    FunctionSymbolTable->addEntry(newVariable->getIdentifier(), newVariable);
}

std::unique_ptr<ValueNode> FunctionSymbol::CallFunction(
        const std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments,
        QSharedPointer<SymbolTable> CurrentSymbolTable
        )
{
    //ToDO SOMEHOW SAVE THE VALUES AND REPRODUCE THEM WHEN THE FUNCTION RETURNS...
//    QSharedPointer<SymbolTable> SavedFunctionSymbolTable = FunctionSymbolTable.data();
    const size_t NrOfParameters = FunctionParameters.size();
    if(NrOfParameters != FunctionArguments.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return std::unique_ptr<ValueNode>( new ValueNode());
    }

    //Is this really needed?!
    //qSharedPointerDynamicCast<SymbolTable>(CurrentSymbolTable)->addEntry(FunctionSymbolTable->getIdentifier(), SymbolTablePtr(&FunctionSymbolTable));

    for(size_t i = 0; i < NrOfParameters; i++)
    {
        QSharedPointer<VariableSymbol> &param = FunctionParameters.at(i);
        const std::unique_ptr<SimpleNode> &argument = FunctionArguments.at(i);
        param->assignValue(*(argument->visit().get()));
    }

    FunctionSymbolTable->addParentSymbolTable(CurrentSymbolTable);

    for(std::unique_ptr<SimpleNode> &expression : FunctionExpressions)
    {
        expression->visit();
    }

    return FunctionReturnNode->visit();
}

QSharedPointer<SymbolTable> FunctionSymbol::getFunctionSymbolTable() const
{
    return FunctionSymbolTable;
}

bool FunctionSymbol::checkFunctionArguments(const QVector<std::unique_ptr<SimpleNode>> &FunctionArguments) const
{
    const int NrOfParameters = FunctionParameters.size();
    if(NrOfParameters != FunctionArguments.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return false;
    }

    for(int i = 0; i < NrOfParameters; i++)
    {
        const QSharedPointer<VariableSymbol> &param = FunctionParameters.at(i);
        const std::unique_ptr<SimpleNode> &argument = FunctionArguments.at(i);
        if(!SimpleNode::canConvertTypes(param->getReturnType(), argument->getReturnType()))
        {
            qDebug() << "Type mismatch at paramter: " << i << " Expected: "
                     << SimpleNode::getHumanReadableTypeNameToValueType(param->getReturnType())
                     << " but got: "
                     << SimpleNode::getHumanReadableTypeNameToValueType(argument->getReturnType());
            return false;
        }
    }

    return true;
}

SymbolTableEntry::SymbolTableEntryType FunctionSymbol::getType() const
{
    return SymbolTableEntry::Function;
}

//ToDO !!!
QString FunctionSymbol::PrintToSymbolToString() const
{
    QString symbolAsString(QString("%1 %2(").arg(SimpleNode::getHumanReadableTypeNameToValueType(getReturnType())).arg(getIdentifier()));

    for(const QSharedPointer<VariableSymbol> &param : FunctionParameters)
    {
        symbolAsString.append(QString(" %1").arg(SimpleNode::getHumanReadableTypeNameToValueType(param->getReturnType())));
    }
    symbolAsString.append(" )");

    return symbolAsString;
}

QString FunctionSymbol::PrintSymbolType() const
{
    return QString("Function");
}

Node::ValueTypes FunctionSymbol::getReturnType() const
{
    return ReturnType;
}

