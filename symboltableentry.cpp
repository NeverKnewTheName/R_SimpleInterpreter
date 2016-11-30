#include "symboltableentry.h"

#include <QDebug>

SymbolTableEntry::SymbolTableEntry(const QString &identifier) :
    identifier(identifier)
{

}

SymbolTableEntry::~SymbolTableEntry()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString SymbolTableEntry::getIdentifier() const
{
    return identifier;
}
