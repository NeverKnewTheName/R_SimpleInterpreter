#ifndef SYMBOL_H
#define SYMBOL_H

#include <QDebug>

#include "symboltableentry.h"
#include "simplenode.h"

class Symbol : public SymbolTableEntry
{
public:
    Symbol(const QString &identifier) : SymbolTableEntry(identifier){}
    virtual ~Symbol(){
        qDebug() << __PRETTY_FUNCTION__;
    }

    virtual Node::ValueTypes getReturnType() const = 0;

    // SymbolTableEntry interface
public:
    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
};


#endif // SYMBOL_H
