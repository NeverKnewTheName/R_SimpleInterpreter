#include "symboltable.h"

#include "simplenode.h"

#include <QDebug>


SymbolTable::SymbolTable(QString const& identifier, QSharedPointer<SymbolTable> parentSymbolTable) :
    SymbolTableEntry(identifier),
    parentSymbolTable(parentSymbolTable)
{
//    if(!parentSymbolTable.isNull())
//    {
//        parentSymbolTable->addEntry(identifier, QSharedPointer<SymbolTableEntry>(this));
//    }
}

SymbolTable::~SymbolTable()
{
    SymbolTableEntries.clear();
    SymbolTableIndices.clear();
    if(!parentSymbolTable.isNull())
    {
        parentSymbolTable->removeEntry(identifier);
    }
    qDebug() << __PRETTY_FUNCTION__;
}

QSharedPointer<SymbolTableEntry> SymbolTable::lookup(const QString &identifier)
{
    QSharedPointer<SymbolTableEntry> entry = QSharedPointer<SymbolTableEntry>();

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

bool SymbolTable::addEntry(const QString &identifier, QSharedPointer<SymbolTableEntry> entry)
{
    if(SymbolTableIndices.contains(identifier))
    {
        qDebug() << "Identifier already taken!";
        return false;
    }

//    if(entry->getType() == SymbolTableEntry::SubSymbolTable)
//    {
//        bool SetAsParentSuccess;
//        SetAsParentSuccess = qSharedPointerDynamicCast<SymbolTable>(entry)->addParentSymbolTable(QSharedPointer<SymbolTable>(this));
//        if(!SetAsParentSuccess)
//        {
//            qDebug() << "Set as parent failed!";
//            return false;
//        }
//    }

    SymbolTableEntries.push_back(entry);
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

    SymbolTableIndices.remove(identifier);
    SymbolTableEntries.erase(SymbolTableEntries.begin() + SymbolIndexToRemove);

    return true;
}

bool SymbolTable::DoesIdentifierExistInCurrentScope(const QString &IdentifierToCheck) const
{
    return SymbolTableIndices.contains(IdentifierToCheck);
}

const std::vector<QSharedPointer<SymbolTableEntry>> &SymbolTable::getSymbolTableEntries() const
{
    return SymbolTableEntries;
}

bool SymbolTable::addParentSymbolTable(QSharedPointer<SymbolTable> parent)
{
    if(parentSymbolTable != NULL)
    {
        qDebug() << "SymbolTable ALREADY HAS A PARENT!";
        return false;
    }
    parentSymbolTable = parent;
    return true;
}

bool SymbolTable::addSubSymbolTable(QSharedPointer<SymbolTable> SubSymbolTable)
{
    return addEntry(SubSymbolTable->getIdentifier(), qSharedPointerDynamicCast<SymbolTableEntry>(SubSymbolTable));
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

QSharedPointer<SymbolTableEntry> SymbolTable::getParentSymbolTable() const
{
    return parentSymbolTable;
}
