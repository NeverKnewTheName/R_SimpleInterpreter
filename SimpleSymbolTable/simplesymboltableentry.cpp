#include "simplesymboltableentry.h"

#include <QDebug>

SimpleSymbolTableEntry::SimpleSymbolTableEntry(const QString &identifier) :
    identifier(identifier)
{

}

SimpleSymbolTableEntry::~SimpleSymbolTableEntry()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QString SimpleSymbolTableEntry::getIdentifier() const
{
    return identifier;
}
