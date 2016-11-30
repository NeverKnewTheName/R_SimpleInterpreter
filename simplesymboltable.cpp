#include "simplesymboltable.h"

#include "simplenode.h"

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
        parentSymbolTable->removeEntry(identifier);
    }
    qDebug() << __PRETTY_FUNCTION__;
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

bool SimpleSymbolTable::addEntry(const QString &identifier, QSharedPointer<SimpleSymbolTableEntry> entry)
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
    return addEntry(SubSymbolTable->getIdentifier(), qSharedPointerDynamicCast<SimpleSymbolTableEntry>(SubSymbolTable));
}

SimpleSymbolTableEntry::SymbolTableEntryType SimpleSymbolTable::getType() const
{
    return SimpleSymbolTableEntry::SubSymbolTable;
}

QString SimpleSymbolTable::PrintToSymbolToString() const
{
    return identifier;
}

QString SimpleSymbolTable::PrintSymbolType() const
{
    return QString("SubSymbolTable");
}

QSharedPointer<SimpleSymbolTableEntry> SimpleSymbolTable::getParentSymbolTable() const
{
    return parentSymbolTable;
}
