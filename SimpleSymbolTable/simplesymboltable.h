#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QString>

#include <QSharedPointer>

#include "simplesymboltableentry.h"

class SimpleStack;

class SimpleSymbolTable : public SimpleSymbolTableEntry
{
public:
    SimpleSymbolTable(QString const& identifier, QSharedPointer<SimpleSymbolTable> parentSymbolTable = QSharedPointer<SimpleSymbolTable>());
    ~SimpleSymbolTable();

    QSharedPointer<SimpleSymbolTableEntry> lookup(QString const& identifier);
    bool addEntry(QSharedPointer<SimpleSymbolTableEntry> entry);
    bool removeEntry(const QString &identifier);

    bool DoesIdentifierExistInCurrentScope(const QString &IdentifierToCheck) const;

    const std::vector<QSharedPointer<SimpleSymbolTableEntry>> &getSymbolTableEntries() const;

    bool addParentSymbolTable(QSharedPointer<SimpleSymbolTable> parent);
    bool addSubSymbolTable(QSharedPointer<SimpleSymbolTable> SubSymbolTable);
    bool removeSubSymbolTable(const QString &identifier);

    QSharedPointer<SimpleSymbolTableEntry> getParentSymbolTable() const;

    bool EnterScope(QSharedPointer<SimpleStack> StackToUse);
    bool ExitScope(QSharedPointer<SimpleStack> StackToUse);

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    QHash<QString,int> SymbolTableIndices;
    std::vector<QSharedPointer<SimpleSymbolTableEntry>> SymbolTableEntries;
    QSharedPointer<SimpleSymbolTable> parentSymbolTable; // DO NOT TOUCH PARENT SYMBOL TABLE
};

#endif // SIMPLESYMBOLTABLE_H
