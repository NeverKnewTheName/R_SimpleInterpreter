#ifndef SIMPLESYMBOL_H
#define SIMPLESYMBOL_H

#include <QDebug>

#include "simplesymboltableentry.h"
#include "simplenode.h"

class SimpleSymbol : public SimpleSymbolTableEntry
{
public:
    SimpleSymbol(const QString &identifier) : SimpleSymbolTableEntry(identifier){}
    virtual ~SimpleSymbol(){
        qDebug() << __PRETTY_FUNCTION__;
    }

    virtual Node::ValueTypes getReturnType() const = 0;

    // SymbolTableEntry interface
public:
    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
};


#endif // SIMPLESYMBOL_H
