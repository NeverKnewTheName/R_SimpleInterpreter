#include "simplesymboltable.h"

#include "simpleast.h"

#include <QDebug>

SymbolTable::SymbolTable(SymbolTable *parentSymbolTable) :
    parentSymbolTable(parentSymbolTable)
{

}

SymbolTable::~SymbolTable()
{
   qDebug() << __PRETTY_FUNCTION__;
}

SymbolTableEntry *SymbolTable::lookup(const QString &identifier)
{
    SymbolTableEntry *entry = NULL;

    if(symblTbl.contains(identifier))
    {
        entry = symblTbl[identifier];
    }
    else if(parentSymbolTable != NULL)
    {
        entry = parentSymbolTable->lookup(identifier);
    }

    return entry;
}

bool SymbolTable::addEntry(const QString &identifier, SymbolTableEntry *entry)
{
    if(symblTbl.contains(identifier))
        return false;

    if(entry->getType() == SymbolTableEntry::SubSymbolTable)
    {
        dynamic_cast<SymbolTable*>(entry)->addParentSymbolTable(this);
    }

    symblTbl[identifier] = entry;
//    WholeSymbolTableAsSequence.append(entry);
    return true;
}

bool SymbolTable::removeEntry(const QString &identifier)
{
    if(!symblTbl.contains(identifier))
        return false;
    SymbolTableEntry *entry = lookup(identifier);

    symblTbl.remove(identifier);
//    WholeSymbolTableAsSequence.removeAll(entry);

    return true;
}

//QVector<SymbolTableEntry *> SymbolTable::getWholeSymbolTableAsSequence()
//{
//    return WholeSymbolTableAsSequence;
//}

void SymbolTable::addParentSymbolTable(SymbolTable * const parent)
{
    if(parentSymbolTable != NULL)
    {
        qDebug() << "SymbolTable ALREADY HAS A PARENT!";
        return;
    }
    parentSymbolTable = parent;
}

SymbolTableEntry::SymbolTableEntryType SymbolTable::getType() const
{
    return SymbolTableEntry::SubSymbolTable;
}

VariableSymbol::VariableSymbol(SimpleNode::ValueTypes VariableType, SimpleNode *ValueNodeForEntry) :
    valueNode(ValueNodeForEntry == NULL ? NULL : new ValueNode(ValueNodeForEntry->visit())),
    VariableType(VariableType)
//    isAssigned(ValueNodeForEntry == NULL ? false : true)
{

}

VariableSymbol::~VariableSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ValueNode *VariableSymbol::getValueNode() const
{
    return valueNode;
}

void VariableSymbol::assignValue(SimpleNode *AssignmentNode)
{
    ValueNode *newValueNode;// = valueNode; //Save the previous value because it could be referenced in the assignment!!!

    if(AssignmentNode == NULL)
    {
        qDebug() << "ERROR: Value to assign is NULL";
        return;
    }
    switch(VariableType)
    {
    case SimpleNode::Integer:
        newValueNode = new ValueNode(AssignmentNode->visit().getValue().value<int>());
        break;
    case SimpleNode::Double:
        newValueNode = new ValueNode(AssignmentNode->visit().getValue().value<double>());
        break;
    case SimpleNode::Bool:
        newValueNode = new ValueNode(AssignmentNode->visit().getValue().value<bool>());
        break;
    case SimpleNode::String:
        newValueNode = new ValueNode(AssignmentNode->visit().getValue().value<QString>());
        break;
    case SimpleNode::Void:
    case SimpleNode::ErrorType:
    default:
        newValueNode = new ValueNode();
    }

    if(valueNode != NULL)
    {
        qDebug() << "VariableSymbol reassignment\nWas: " << valueNode->visit().getValue();
        delete valueNode;
    }

    valueNode = newValueNode;

    qDebug() << "Value Assigned: " << valueNode->visit().getValue();
}

SymbolTableEntry::SymbolTableEntryType VariableSymbol::getType() const
{
    return SymbolTableEntry::Variable;
}

SimpleNode::ValueTypes VariableSymbol::getVariableType() const
{
    return VariableType;
}

FunctionSymbol::FunctionSymbol(FunctionNode *FunctionNodeForEntry) :
    functionNode(FunctionNodeForEntry)
{

}

FunctionSymbol::~FunctionSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

FunctionNode *FunctionSymbol::GetFunctionNode() const
{
    return functionNode;
}

SymbolTableEntry::SymbolTableEntryType FunctionSymbol::getType() const
{
    return SymbolTableEntry::Function;
}


SymbolTableEntry::SymbolTableEntry()
{

}


SymbolTableEntry::~SymbolTableEntry()
{
    qDebug() << __PRETTY_FUNCTION__;
}
