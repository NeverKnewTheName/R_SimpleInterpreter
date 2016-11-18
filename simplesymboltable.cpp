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
    return symblTbl[identifier];
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
////    return WholeSymbolTableAsSequence;
//}

void SymbolTable::addParentSymbolTable(SymbolTable * const parent)
{
    if(parentSymbolTable == NULL)
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

VariableSymbol::VariableSymbol(SimpleNode *ValueNodeForEntry) :
    valueNode(ValueNodeForEntry == NULL ? NULL : new ValueNode(ValueNodeForEntry->visit()))
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
    if(valueNode != NULL)
    {
        qDebug() << "VariableSymbol reassignment\nWas: " << valueNode->visit().getValue();
        delete valueNode;
    }

    if(AssignmentNode == NULL)
    {
        qDebug() << "ERROR: Value to assign is NULL";
        return;
    }

    valueNode = new ValueNode(AssignmentNode->visit());
    qDebug() << "Value Assigned: " << valueNode->visit().getValue();
}

SymbolTableEntry::SymbolTableEntryType VariableSymbol::getType() const
{
    return SymbolTableEntry::Variable;
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
