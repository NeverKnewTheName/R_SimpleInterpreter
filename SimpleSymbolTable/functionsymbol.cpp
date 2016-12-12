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
    FunctionStatement(new VoidValueNode())
{
    const int NrOfFunctionParameters = FunctionParameters.size();
    for(int i = 0; i < NrOfFunctionParameters; i++)
    {
        //ToTHINK is this needed??? Actually the parser does this beforehand!...
        FunctionSymbolTable->addEntry(qSharedPointerDynamicCast<SimpleSymbolTableEntry>(FunctionParameters.at(i)));
    }
}

FunctionSymbol::~FunctionSymbol()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QSharedPointer<SimpleSymbolTable> FunctionSymbol::getFunctionSymbolTable() const
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

const std::vector<QSharedPointer<VariableSymbol> > &FunctionSymbol::getFunctionParameters() const
{
    return FunctionParameters;
}

const std::unique_ptr<SimpleNode> &FunctionSymbol::getFunctionStatement() const
{
    return FunctionStatement;
}

void FunctionSymbol::setFunctionStatement(std::unique_ptr<SimpleNode> value)
{
    FunctionStatement.reset(value.release());
}

