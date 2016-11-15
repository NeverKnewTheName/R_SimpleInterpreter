#include "simplesymboltable.h"

#include <QDebug>

SymbolTableEntry::SymbolTableEntry() :
    SymblEntryType(SymbolTableEntry::ERRORType),
    SymblEntryValue(QVariant())
{
}

SymbolTableEntry::SymbolTableEntry(SymbolTable * const symblTbl) :
    SymblEntryType(SymbolTableEntry::SubSymbolTable),
    SymblEntryValue(QVariant::fromValue(static_cast<void*>(symblTbl)))
{
}

SymbolTableEntry::SymbolTableEntry(int IntegerValue) :
    SymblEntryType(SymbolTableEntry::Integer),
    SymblEntryValue(QVariant::fromValue(IntegerValue))
{
}

SymbolTableEntry::SymbolTableEntry(double DoubleValue) :
    SymblEntryType(SymbolTableEntry::Double),
    SymblEntryValue(QVariant::fromValue(DoubleValue))
{
}

SymbolTableEntry::SymbolTableEntry(bool BoolValue) :
    SymblEntryType(SymbolTableEntry::Bool),
    SymblEntryValue(QVariant::fromValue(BoolValue))
{
}

SymbolTableEntry::SymbolTableEntry(QString StringValue) :
    SymblEntryType(SymbolTableEntry::String),
    SymblEntryValue(QVariant::fromValue(StringValue))
{
}

SymbolTableEntry::SymbolTableEntry(const SymbolTableEntry &SymblTblEntry) :
    SymblEntryType(SymblTblEntry.getSymbolTableEntryType()),
    SymblEntryValue(QVariant(SymblTblEntry.getSymbolTableEntryValue()))
{
}

SymbolTableEntry::SymbolTableEntryType SymbolTableEntry::getSymbolTableEntryType() const
{
    return SymblEntryType;
}

const QVariant &SymbolTableEntry::getSymbolTableEntryValue() const
{
    return SymblEntryValue;
}

void SymbolTableEntry::SetSymbolTableEntryValue(int IntegerValue)
{
    if(SymblEntryType != SymbolTableEntry::Integer)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Integer;
        return;
    }
    SymblEntryValue = QVariant::fromValue(IntegerValue);
}

void SymbolTableEntry::SetSymbolTableEntryValue(double DoubleValue)
{
    if(SymblEntryType != SymbolTableEntry::Double)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Double;
        return;
    }
    SymblEntryValue = QVariant::fromValue(DoubleValue);
}

void SymbolTableEntry::SetSymbolTableEntryValue(bool BoolValue)
{
    if(SymblEntryType != SymbolTableEntry::Bool)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Bool;
        return;
    }
    SymblEntryValue = QVariant::fromValue(BoolValue);
}

void SymbolTableEntry::SetSymbolTableEntryValue(QString StringValue)
{
    if(SymblEntryType != SymbolTableEntry::String)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::String;
        return;
    }
    SymblEntryValue = QVariant::fromValue(StringValue);
}

SymbolTable::SymbolTable()
{
}

SymbolTableEntry &SymbolTable::lookup(const QString &identifier)
{
    return symblTbl[identifier];
}

bool SymbolTable::addEntry(const QString &identifier, SymbolTableEntry entry)
{
    if(symblTbl.contains(identifier))
        return false;

    symblTbl[identifier] = entry;
    return true;
}
