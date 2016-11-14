#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QVariant>
#include <QString>

class SymbolTable;

class SymbolTableEntry
{
public:
    typedef enum _SymbolTableEntryType
    {
        SubSymbolTable,
        Integer,
        Double,
        Bool,
        String,
        ERRORType
    }SymbolTableEntryType;

    SymbolTableEntry();
    SymbolTableEntry(SymbolTable * const symblTbl);
    SymbolTableEntry(int IntegerValue);
    SymbolTableEntry(double DoubleValue);
    SymbolTableEntry(bool BoolValue);
    SymbolTableEntry(QString StringValue);
    SymbolTableEntry(SymbolTableEntry const &SymblTblEntry);

    SymbolTableEntryType getSymbolTableEntryType() const;
    QVariant const &getSymbolTableEntryValue() const;
    bool setSymbolTableEntryValue(QVariant &valueToSet);

private:
    SymbolTableEntryType SymblEntryType;
    QVariant SymblEntryValue;
};

class SymbolTable
{
public:
    SymbolTable();
    SymbolTableEntry &lookup(QString const& identifier);
    bool addEntry(QString const& identifier, SymbolTableEntry entry);
    bool removeEntry(QString const& identifier, SymbolTableEntry entry);
private:
    QHash<QString,SymbolTableEntry> symblTbl;
};

#endif // SIMPLESYMBOLTABLE_H
