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
    SymblEntryValue(QVariant(SymblTblValue.getSymbolTableEntryValue()))
{
}

SymbolTableEntry::SymbolTableEntry(SymbolTable * const symblTbl) :
    SymblEntryType(SymbolTableEntry::SubSymbolTable),
    SymblEntryValue(QVariant::fromValue(static_cast<void*>(symblTbl)))
{
}

SymbolTableEntry::SymbolTableEntryType SymbolTableEntry::getSymbolTableType() const
{
    return SymblEntryType;
}

const QVariant &SymbolTableEntry::getSymbolTableValue() const
{
    return SymblEntryValue;
}

void SymbolTableEntry::SetSymbolTableEntry(int IntegerValue)
{
    if(SymblEntryType != SymbolTableEntry::Integer)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Integer;
        return;
    }
    SymblEntryValue = QVariant::fromValue(IntegerValue);
}

void SymbolTableEntry::SetSymbolTableEntry(double DoubleValue)
{
    if(SymblEntryType != SymbolTableEntry::Double)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Double;
        return;
    }
    SymblEntryValue = QVariant::fromValue(DoubleValue);
}

void SymbolTableEntry::SetSymbolTableEntry(bool BoolValue)
{
    if(SymblEntryType != SymbolTableEntry::Bool)
    {
        qDebug() << __PRETTY_FUNCTION__ << "ERROR" << " TypeMismatch: " << "is " << SymblEntryType << "; tried to assign " << SymbolTableEntry::Bool;
        return;
    }
    SymblEntryValue = QVariant::fromValue(BoolValue);
}

void SymbolTableEntry::SetSymbolTableEntry(QString StringValue)
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
