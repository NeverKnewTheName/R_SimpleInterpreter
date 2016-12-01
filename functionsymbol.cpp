#include "functionsymbol.h"

#include <QDebug>

#include "simplesymboltable.h"
#include "variablesymbol.h"
#include "simplestack.h"

FunctionSymbol::FunctionSymbol(
        const QString &identifier,
        QSharedPointer<SimpleSymbolTable> SymbolTableForFunction,
        std::vector<QSharedPointer<VariableSymbol> > &&functionParameters,
        Node::ValueTypes ReturnType
        ) :
    SimpleSymbol(identifier),
    FunctionSymbolTable(SymbolTableForFunction),
    ReturnType(ReturnType),
    FunctionParameters(functionParameters),
    FunctionReturnNode(new ValueNode())
{
    const int NrOfFunctionParameters = FunctionParameters.size();
    for(int i = 0; i < NrOfFunctionParameters; i++)
    {
        QString paramIdentifier = FunctionParameters.at(i)->getIdentifier();
        FunctionSymbolTable->addEntry(qSharedPointerDynamicCast<SimpleSymbolTableEntry>(FunctionParameters.at(i)));
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

//void FunctionSymbol::addVariableDefinition(QSharedPointer<VariableSymbol> newVariable)
//{
//    FunctionSymbolTable->addEntry(newVariable);
//}

std::unique_ptr<ValueNode> FunctionSymbol::CallFunction(
        const std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments,
        QSharedPointer<SimpleStack> StackToUse
        )
{
    const size_t NrOfParameters = FunctionParameters.size();

    if(NrOfParameters != FunctionArguments.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return std::unique_ptr<ValueNode>( new ValueNode());
    }

    std::vector<std::unique_ptr<ValueNode>> EvaluatedFuncArgs;
    for(size_t i = 0; i < NrOfParameters; i++)
    {
        EvaluatedFuncArgs.emplace_back(std::move(FunctionArguments.at(i)->visit(StackToUse)));
    }
    FunctionSymbolTable->EnterScope(StackToUse);

    for(size_t i = 0; i < NrOfParameters; i++)
    {
        QSharedPointer<VariableSymbol> &param = FunctionParameters.at(i);
        std::unique_ptr<ValueNode> &argument = EvaluatedFuncArgs.at(i);
        param->assignValue(std::move(argument), StackToUse);
    }


    for(std::unique_ptr<SimpleNode> &expression : FunctionExpressions)
    {
        expression->visit(StackToUse);
    }

    std::unique_ptr<ValueNode> returnValue = FunctionReturnNode->visit(StackToUse);

    FunctionSymbolTable->ExitScope(StackToUse);
    return returnValue;
}

QSharedPointer<SimpleSymbolTable> FunctionSymbol::getFunctionSymbolTable() const
{
    return FunctionSymbolTable;
}

bool FunctionSymbol::BuildFunctionStack(QSharedPointer<SimpleStack> StackToUse) const
{
    bool IsSuccessfull = true;
    const std::vector<QSharedPointer<SimpleSymbolTableEntry>> SymbolTableEntries = FunctionSymbolTable->getSymbolTableEntries();

    for(auto & symbol: SymbolTableEntries)
    {
        if(symbol->getType() == SimpleSymbolTableEntry::Variable)
        {
            IsSuccessfull = IsSuccessfull || qSharedPointerDynamicCast<VariableSymbol>(symbol)->VarEnterScope(StackToUse);
        }
    }

    return IsSuccessfull;
}

bool FunctionSymbol::DestroyFunctionStack(QSharedPointer<SimpleStack> StackToUse) const
{
    bool IsSuccessfull = true;
    const std::vector<QSharedPointer<SimpleSymbolTableEntry>> SymbolTableEntries = FunctionSymbolTable->getSymbolTableEntries();

    for(auto & symbol: SymbolTableEntries)
    {
        if(symbol->getType() == SimpleSymbolTableEntry::Variable)
        {
            IsSuccessfull = IsSuccessfull || qSharedPointerDynamicCast<VariableSymbol>(symbol)->VarEnterScope(StackToUse);
        }
    }

    return IsSuccessfull;
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

SimpleSymbolTableEntry::SymbolTableEntryType FunctionSymbol::getType() const
{
    return SimpleSymbolTableEntry::Function;
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

