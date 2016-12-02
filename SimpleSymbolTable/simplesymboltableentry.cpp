#include "simplesymboltableentry.h"

#include <QDebug>

SimpleSymbolTableEntry::SimpleSymbolTableEntry(const QString &identifier) :
    identifier(identifier)
{

}

SimpleSymbolTableEntry::~SimpleSymbolTableEntry()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString SimpleSymbolTableEntry::getIdentifier() const
{
    return identifier;
}
