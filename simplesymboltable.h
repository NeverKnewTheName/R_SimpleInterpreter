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
protected:
    bool isAssigned;
};

class SymbolTable : public SymbolTableEntry
{
public:
    SymbolTable(SymbolTable *parentSymbolTable = NULL);
    ~SymbolTable();

    SymbolTableEntry *lookup(QString const& identifier);
    bool addEntry(QString const& identifier, SymbolTableEntry *entry);
    bool removeEntry(QString const& identifier);

//    QVector<SymbolTableEntry *> getWholeSymbolTableAsSequence();

    void addParentSymbolTable(SymbolTable * const parent);

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;

private:
    QHash<QString,SymbolTableEntry*> symblTbl;
//    static QVector<SymbolTableEntry *> WholeSymbolTableAsSequence;
    SymbolTable *parentSymbolTable;
};

class VariableSymbol : public SymbolTableEntry
{
public:
    VariableSymbol(SimpleNode::ValueTypes VariableType = SimpleNode::Integer, SimpleNode *ValueNodeForEntry = NULL);
    ~VariableSymbol();

    ValueNode *getValueNode() const;

    void assignValue(SimpleNode *AssignmentNode);

    SimpleNode::ValueTypes getVariableType() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;

private:
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

private:
    FunctionNode *functionNode;
};

#endif // SIMPLESYMBOLTABLE_H
