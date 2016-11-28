#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QVariant>
#include <QString>

#include "simpleast.h"

//class SimpleNode;
//class ValueNode;
//class FunctionNode;

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

    SymbolTableEntry();
    virtual ~SymbolTableEntry();

    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
protected:
    bool isAssigned;
};

class SymbolTable : public SymbolTableEntry
{
public:
    SymbolTable(QString const& identifier, SymbolTable *parentSymbolTable = NULL);
    ~SymbolTable();

    SymbolTableEntry *lookup(QString const& identifier);
    bool addEntry(QString const& identifier, SymbolTableEntry *entry);
    bool removeEntry(QString const& identifier);

    QVector<SymbolTableEntry *> getSymbolTableAsSequence();

    void addParentSymbolTable(SymbolTable * const parent);

    SymbolTable *getParentSymbolTable() const;

    QString getIdentifier() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    QString identifier;
    QHash<QString,SymbolTableEntry*> symblTbl;
    QVector<SymbolTableEntry *> SymbolTableAsSequence;
    SymbolTable *parentSymbolTable;
};

class VariableSymbol : public SymbolTableEntry
{
public:
    VariableSymbol(const QString &identifier, SimpleNode::ValueTypes VariableType = SimpleNode::Integer, SimpleNode *ValueNodeForEntry = NULL);
    ~VariableSymbol();

    ValueNode *getAssignedValue() const;

    void assignValue(SimpleNode *NodeToAssign);

    SimpleNode::ValueTypes getVariableType() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    QString identifier;
    ValueNode *valueNode;
    SimpleNode::ValueTypes VariableType;
};

class FunctionSymbol : public SymbolTableEntry
{
public:
    FunctionSymbol(FunctionNode *FunctionNodeForEntry = NULL);
    ~FunctionSymbol();

    FunctionNode *GetFunctionNode() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    FunctionNode *functionNode;
};

#endif // SIMPLESYMBOLTABLE_H
