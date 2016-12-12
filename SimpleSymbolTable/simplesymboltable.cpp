#include "simplesymboltable.h"

#include "simplenode.h"

#include "simplesymboltableentry.h"
#include "variablesymbol.h"

#include "simplestack.h"

#include <QDebug>


SimpleSymbolTable::SimpleSymbolTable(QString const& identifier, QSharedPointer<SimpleSymbolTable> parentSymbolTable) :
    SimpleSymbolTableEntry(identifier),
    parentSymbolTable(parentSymbolTable)
{
//    if(!parentSymbolTable.isNull())
//    {
//        parentSymbolTable->addEntry(identifier, QSharedPointer<SymbolTableEntry>(this));
//    }
}

SimpleSymbolTable::~SimpleSymbolTable()
{
    SymbolTableEntries.clear();
    SymbolTableIndices.clear();
    if(!parentSymbolTable.isNull())
    {
        parentSymbolTable->removeEntry(getIdentifier());
    }
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QSharedPointer<SimpleSymbolTableEntry> SimpleSymbolTable::lookup(const QString &identifier)
{
    QSharedPointer<SimpleSymbolTableEntry> entry = QSharedPointer<SimpleSymbolTableEntry>();

    if(SymbolTableIndices.contains(identifier))
    {
        entry = SymbolTableEntries.at(SymbolTableIndices[identifier]);
    }
    else if(!parentSymbolTable.isNull())
    {
        entry = parentSymbolTable->lookup(identifier);
    }

    return entry;
}

bool SimpleSymbolTable::addEntry(QSharedPointer<SimpleSymbolTableEntry> entry)
{
    const QString &identifier = entry->getIdentifier();
    if(SymbolTableIndices.contains(identifier))
    {
        qDebug() << "Identifier already taken!";
        return false;
    }

    SymbolTableIndices[identifier] = SymbolTableEntries.size();
    SymbolTableEntries.push_back(entry);

    return true;
}

bool SimpleSymbolTable::removeEntry(const QString &identifier)
{
    if(!SymbolTableIndices.contains(identifier))
    {
        qDebug() << "Identifier not found!";
        return false;
    }

    int SymbolIndexToRemove = SymbolTableIndices.value(identifier);

    SymbolTableIndices.remove(identifier);
    SymbolTableEntries.erase(SymbolTableEntries.begin() + SymbolIndexToRemove);

    return true;
}

bool SimpleSymbolTable::DoesIdentifierExistInCurrentScope(const QString &IdentifierToCheck) const
{
    return SymbolTableIndices.contains(IdentifierToCheck);
}

const std::vector<QSharedPointer<SimpleSymbolTableEntry>> &SimpleSymbolTable::getSymbolTableEntries() const
{
    return SymbolTableEntries;
}

bool SimpleSymbolTable::addParentSymbolTable(QSharedPointer<SimpleSymbolTable> parent)
{
    if(parentSymbolTable != NULL)
    {
        qDebug() << "SymbolTable ALREADY HAS A PARENT!";
        return false;
    }
    parentSymbolTable = parent;
    return true;
}

bool SimpleSymbolTable::addSubSymbolTable(QSharedPointer<SimpleSymbolTable> SubSymbolTable)
{
    return addEntry(qSharedPointerDynamicCast<SimpleSymbolTableEntry>(SubSymbolTable));
}

bool SimpleSymbolTable::removeSubSymbolTable(const QString &identifier)
{
    SimpleSymbolTableEntry::SymbolTableEntryType type = lookup(identifier)->getType();
    if(type != SimpleSymbolTableEntry::SubSymbolTable || type != SimpleSymbolTableEntry::Function)
    {
        return false;
    }

    return removeEntry(identifier);
}

SimpleSymbolTableEntry::SymbolTableEntryType SimpleSymbolTable::getType() const
{
    return SimpleSymbolTableEntry::SubSymbolTable;
}

QString SimpleSymbolTable::PrintToSymbolToString() const
{
    return getIdentifier();
}

QString SimpleSymbolTable::PrintSymbolType() const
{
    return QString("SubSymbolTable");
}

QSharedPointer<SimpleSymbolTableEntry> SimpleSymbolTable::getParentSymbolTable() const
{
    return parentSymbolTable;
}

bool SimpleSymbolTable::EnterScope(QSharedPointer<SimpleStack> StackToUse)
{
    for( auto &entry : SymbolTableEntries )
    {
        if(entry->getType() == SimpleSymbolTableEntry::Variable)
        {
            qSharedPointerDynamicCast<VariableSymbol>(entry)->VarEnterScope(StackToUse);
        }
    }
    return true;
}

bool SimpleSymbolTable::ExitScope(QSharedPointer<SimpleStack> StackToUse)
{

    for( auto &entry : SymbolTableEntries )
    {
        if(entry->getType() == SimpleSymbolTableEntry::Variable)
        {
            qSharedPointerDynamicCast<VariableSymbol>(entry)->VarExitScope(StackToUse);
        }
    }
    return true;
}
