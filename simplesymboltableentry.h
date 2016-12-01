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
        ConstantValue,
        Function,
        Keyword,
        ERRORType
    }SymbolTableEntryType;

    SimpleSymbolTableEntry(const QString & identifier);
    virtual ~SimpleSymbolTableEntry();

    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
    QString getIdentifier() const;

private:
    QString identifier;
};

#endif // SIMPLESYMBOLTABLEENTRY_H
