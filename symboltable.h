#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QHash>
#include <QString>

#include <QSharedPointer>

#include "symboltableentry.h"

class SymbolTable : public SymbolTableEntry
{
public:
    SymbolTable(QString const& identifier, QSharedPointer<SymbolTable> parentSymbolTable = QSharedPointer<SymbolTable>());
    ~SymbolTable();

    QSharedPointer<SymbolTableEntry> lookup(QString const& identifier);
    bool addEntry(QString const& identifier, QSharedPointer<SymbolTableEntry> entry);
    bool removeEntry(QString const& identifier);

    const std::vector<QSharedPointer<SymbolTableEntry>> &getSymbolTableEntries() const;

    bool addParentSymbolTable(QSharedPointer<SymbolTable> parent);
    bool addSubSymbolTable(QSharedPointer<SymbolTable> SubSymbolTable);

    QSharedPointer<SymbolTableEntry> getParentSymbolTable() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    QHash<QString,int> SymbolTableIndices;
    std::vector<QSharedPointer<SymbolTableEntry>> SymbolTableEntries;
    QSharedPointer<SymbolTable> parentSymbolTable; // DO NOT TOUCH PARENT SYMBOL TABLE
};

#endif // SYMBOLTABLE_H
