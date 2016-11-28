#include "simplesymboltable.h"

#include "simpleast.h"

#include <QDebug>


SymbolTable::SymbolTable(QString const& identifier, SymbolTable *parentSymbolTable) :
    identifier(identifier),
    parentSymbolTable(parentSymbolTable)
{
    if(parentSymbolTable != NULL)
    {
        parentSymbolTable->addEntry(identifier, this);
    }
}

SymbolTable::~SymbolTable()
{
    for(SymbolTableEntry *entry : this->SymbolTableAsSequence)
    {
        if(entry == NULL)
            continue;

//        if(entry->getType() == SymbolTableEntry::SubSymbolTable)
//        {
//            SymbolTable *parentSymbolTable = dynamic_cast<SymbolTable*>(entry)->getParentSymbolTable();
//            if(parentSymbolTable != NULL)
//            {
//                parentSymbolTable->removeEntry(dynamic_cast<SymbolTable*>(entry)->getIdentifier());
//            }
//        }
        delete entry;
    }
    SymbolTableAsSequence.clear();
    symblTbl.clear();
    if(getParentSymbolTable() != NULL)
    {
        getParentSymbolTable()->removeEntry(identifier);
    }
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
    SymbolTableAsSequence.append(entry);
    return true;
}

bool SymbolTable::removeEntry(const QString &identifier)
{
    if(!symblTbl.contains(identifier))
        return false;
    SymbolTableEntry *entry = lookup(identifier);

    symblTbl.remove(identifier);
    SymbolTableAsSequence.removeAll(entry);

    return true;
}

QVector<SymbolTableEntry *> SymbolTable::getSymbolTableAsSequence()
{
    return SymbolTableAsSequence;
}

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

SymbolTable *SymbolTable::getParentSymbolTable() const
{
    return parentSymbolTable;
}

VariableSymbol::VariableSymbol(const QString &identifier, SimpleNode::ValueTypes VariableType, SimpleNode *ValueNodeForEntry) :
    identifier(identifier),
    valueNode(ValueNodeForEntry == NULL ? NULL : new ValueNode(ValueNodeForEntry->visit())),
    VariableType(VariableType)
  //    isAssigned(ValueNodeForEntry == NULL ? false : true)
{

}

VariableSymbol::~VariableSymbol()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ValueNode *VariableSymbol::getAssignedValue() const
{
    return valueNode;
}

void VariableSymbol::assignValue(SimpleNode *NodeToAssign)
{
    ValueNode *newValueNode;// = valueNode; //Save the previous value because it could be referenced in the assignment!!!

    if(NodeToAssign == NULL)
    {
        qDebug() << "ERROR: Value to assign is NULL";
        return;
    }
    switch(VariableType)
    {
    case SimpleNode::Integer:
        newValueNode = new ValueNode(NodeToAssign->visit().getValue().value<int>());
        break;
    case SimpleNode::Double:
        newValueNode = new ValueNode(NodeToAssign->visit().getValue().value<double>());
        break;
    case SimpleNode::Bool:
        newValueNode = new ValueNode(NodeToAssign->visit().getValue().value<bool>());
        break;
    case SimpleNode::String:
        newValueNode = new ValueNode(NodeToAssign->visit().getValue().value<QString>());
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
