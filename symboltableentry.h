#ifndef SYMBOLTABLEENTRY_H
#define SYMBOLTABLEENTRY_H

#include <QString>

class SymbolTableEntry
{
public:
    typedef enum _SymbolTableEntryType
    {
        SubSymbolTable,
        Variable,
        Function,
        UNAssigned,
        ERRORType
    }SymbolTableEntryType;

    SymbolTableEntry(QString const& identifier);
    virtual ~SymbolTableEntry();

    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
    QString getIdentifier() const;

protected:
    QString identifier;
    bool isAssigned;
};

#endif // SYMBOLTABLEENTRY_H
