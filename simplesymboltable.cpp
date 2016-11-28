#include "simplesymboltable.h"

#include "simpleast.h"

#include <QDebug>


SymbolTable::SymbolTable(QString const& identifier, SymbolTableEntryPtr parentSymbolTable) :
    identifier(identifier),
    parentSymbolTable(parentSymbolTable)
{
    if(parentSymbolTable != NULL)
    {
        parentSymbolTable->addEntry(identifier, SymbolTableEntryPtr(this));
    }
}

SymbolTable::~SymbolTable()
{
    SymbolTableAsSequence.clear();
    symblTbl.clear();
    if(parentSymbolTable != NULL)
    {
        parentSymbolTable->removeEntry(identifier);
    }
    qDebug() << __PRETTY_FUNCTION__;
}

SymbolTableEntryPtr SymbolTable::lookup(const QString &identifier)
{
    SymbolTableEntryPtr entry = NULL;

    if(symblTbl.contains(identifier))
    {
        entry = SymbolTableEntries.at(SymbolTableIndices[identifier]);
    }
    else if(parentSymbolTable != NULL)
    {
        entry = parentSymbolTable->lookup(identifier);
    }

    return entry;
}

bool SymbolTable::addEntry(const QString &identifier, SymbolTableEntryPtr entry)
{
    if(symblTbl.contains(identifier))
    {
        qDebug() << "Identifier already taken!";
        return false;
    }

    if(entry->getType() == SymbolTableEntry::SubSymbolTable)
    {
        bool SetAsParentSuccess;
        SetAsParentSuccess = qSharedPointerDynamicCast<SymbolTable>(entry)->addParentSymbolTable(SymbolTableEntryPtr(this));
        if(!SetAsParentSuccess)
        {
            qDebug() << "Set as parent failed!";
            return false;
        }
    }

    SymbolTableEntries.append(entry);
    SymbolTableIndices[identifier] = SymbolTableEntries.size()-1;

    return true;
}

bool SymbolTable::removeEntry(const QString &identifier)
{
    if(!SymbolTableIndices.contains(identifier))
    {
        qDebug() << "Identifier not found!";
        return false;
    }

    int SymbolIndexToRemove = SymbolTableIndices.value(identifier);

    symblTbl.remove(identifier);
    SymbolTableEntries.remove(SymbolIndexToRemove);

    return true;
}

QVector<SymbolTableEntryPtr> SymbolTable::getSymbolTableEntries()
{
    return SymbolTableEntries;
}

bool SymbolTable::addParentSymbolTable(const SymbolTableEntryPtr parent)
{
    if(parentSymbolTable != NULL)
    {
        qDebug() << "SymbolTable ALREADY HAS A PARENT!";
        return false;
    }
    parentSymbolTable = parent;
    return true;
}

SymbolTableEntry::SymbolTableEntryType SymbolTable::getType() const
{
    return SymbolTableEntry::SubSymbolTable;
}

QString SymbolTable::PrintToSymbolToString() const
{
    return identifier;
}

QString SymbolTable::PrintSymbolType() const
{
    return QString("SubSymbolTable");
}

QString SymbolTable::getIdentifier() const
{
    return identifier;
}

SymbolTableEntryPtr SymbolTable::getParentSymbolTable() const
{
    return parentSymbolTable;
}

VariableSymbol::VariableSymbol(
        const QString &identifier,
        SimpleNode::ValueTypes VariableType
        ) :
    identifier(identifier),
    AssignedNode(ValueNode()),
    VariableType(VariableType),
    isAssigned(false)
{

}

VariableSymbol::~VariableSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const ValueNode &VariableSymbol::getAssignedValue() const
{
    return valueNode;
}

bool VariableSymbol::assignValue(const SimpleNode &NodeToAssign)
{
    ValueNode newValueNode;// = valueNode; //Save the previous value because it could be referenced in the assignment!!!

    if(!SimpleNode::canConvertTypes(VariableType, NodeToAssign.getReturnType()))
    {
        qDebug() << "ERROR: TypeMismatch";
        return false;
    }

    switch(VariableType)
    {
    case SimpleNode::Integer:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign->visit().getValue().value<int>()));
        break;
    case SimpleNode::Double:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign->visit().getValue().value<double>()));
        break;
    case SimpleNode::Bool:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign->visit().getValue().value<bool>()));
        break;
    case SimpleNode::String:
        newValueNode = std::move(ValueNode(std::move(NodeToAssign->visit().getValue().value<QString>()));
        break;
    case SimpleNode::Void:
    case SimpleNode::ErrorType:
    default:
        newValueNode = std::move(ValueNode());
    }

    qDebug() << "Previous Value: " << AssignedNode.visit().getValue();

    AssignedNode = std::move(newValueNode);

    qDebug() << "Value Assigned: " << AssignedNode.visit().getValue();

    return true;
}

SymbolTableEntry::SymbolTableEntryType VariableSymbol::getType() const
{
    return SymbolTableEntry::Variable;
}

QString VariableSymbol::PrintToSymbolToString() const
{
    return QString("%1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(getVariableType())).arg(identifier);
}

QString VariableSymbol::PrintSymbolType() const
{
    return QString("Variable");
}

SimpleNode::ValueTypes VariableSymbol::getVariableType() const
{
    return VariableType;
}

FunctionSymbol::FunctionSymbol(
        QString &identifier,
        QVector<VariableNode> &FunctionParameters,
        SimpleNode::ValueTypes ReturnType
        ) :
    identifier(std::move(identifier)),
    FunctionParameters(std::move(FunctionParameters)),
    ReturnType(ReturnType),
    FunctionSymbolTable(QString("%1_SymbolTable").arg(identifier)),
    FunctionReturnNode(ValueNode()),
    isAssigned(false)
{
    for(VariableNode &&param : FunctionParameters)
    {
        QString paramIdentifier = param.getVariableName();
        FunctionSymbolTable.addEntry(paramIdentifier,SymbolTableEntryPtr(new VariableSymbol(paramIdentifier, param.getReturnType())));
    }
}

FunctionSymbol::~FunctionSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

void FunctionSymbol::addFunctionExpressions(QVector<SimpleNode> &&FuncExpressions)
{
    FunctionExpressions = std::move(FuncExpressions);
}

void FunctionSymbol::addFunctionReturnStatement(const SimpleNode &returnNode)
{
    FunctionReturnNode = returnNode;
}

ValueNode FunctionSymbol::CallFunction(
        const QVector<SimpleNode> &FunctionArguments,
        SymbolTableEntryPtr CurrentSymbolTable
        )
{
    const int NrOfParameters = FunctionParameters.size();
    if(NrOfParameters != FunctionArguments.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return ValueNode();
    }

    //Is this really needed?!
    qSharedPointerDynamicCast<SymbolTable>(CurrentSymbolTable)->addEntry(FunctionSymbolTable.getIdentifier(), SymbolTablePtr(&FunctionSymbolTable));

    for(int i = 0; i < NrOfParameters; i++)
    {
        const VariableNode &param = FunctionParameters.at(i);
        const SimpleNode &argument = FunctionArguments.at(i);
        qSharedPointerDynamicCast<VariableSymbol>(FunctionSymbolTable.lookup(param.getVariableName()))->assignValue(argument.visit());
    }

    FunctionSymbolTable.addParentSymbolTable(CurrentSymbolTable);

    for(SimpleNode &expression : FunctionExpressions)
    {
        expression.visit();
    }

    return FunctionReturnNode.visit();
}

bool FunctionSymbol::checkFunctionArguments(const QVector<VariableNode> &FunctionArguments) const
{
    const int NrOfParameters = FunctionParameters.size();
    if(NrOfParameters != FunctionArguments.size())
    {
        qDebug() << "Number of passed arguments does not match function parameters!";
        return false;
    }

    for(int i = 0; i < NrOfParameters; i++)
    {
        const VariableNode &param = FunctionParameters.at(i);
        const SimpleNode &argument = FunctionArguments.at(i);
        if(!SimpleNode::canConvertTypes(param.getReturnType(), argument.getReturnType()))
        {
            qDebug() << "Type mismatch at paramter: " << i << " Expected: "
                     << SimpleNode::getHumanReadableTypeNameToValueType(param.getReturnType())
                     << " but got: "
                     << SimpleNode::getHumanReadableTypeNameToValueType(argument.getReturnType());
            return false;
        }
    }

    return true;
}

SymbolTableEntry::SymbolTableEntryType FunctionSymbol::getType() const
{
    return SymbolTableEntry::Function;
}

QString FunctionSymbol::PrintToSymbolToString() const
{
    return functionNode->printValue();
}

QString FunctionSymbol::PrintSymbolType() const
{
    return QString("Function");
}


SymbolTableEntry::SymbolTableEntry()
{

}

SymbolTableEntry::~SymbolTableEntry()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::ValueTypes Symbol::getReturnType() const
{
    return SimpleNode::ErrorType;
}
