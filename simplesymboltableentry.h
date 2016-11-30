#ifndef SIMPLESYMBOLTABLEENTRY_H
#define SIMPLESYMBOLTABLEENTRY_H

#include <QString>

class SimpleSymbolTableEntry
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

    SimpleSymbolTableEntry(QString const& identifier);
    virtual ~SimpleSymbolTableEntry();

    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
    QString getIdentifier() const;

protected:
    QString identifier;
    bool isAssigned;
};

#endif // SIMPLESYMBOLTABLEENTRY_H
