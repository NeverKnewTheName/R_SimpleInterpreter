#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QVariant>
#include <QString>

#include <QSharedPointer>

#include "symbolnodes.h"

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
    QString identifier;
    bool isAssigned;
};

typedef QSharedPointer<SymbolTableEntry> SymbolTableEntryPtr;

class SymbolTable : public SymbolTableEntry
{
public:
    SymbolTable(QString const& identifier, SymbolTableEntryPtr parentSymbolTable = SymbolTableEntryPtr());
    ~SymbolTable();

    SymbolTableEntryPtr lookup(QString const& identifier);
    bool addEntry(QString const& identifier, SymbolTableEntryPtr entry);
    bool removeEntry(QString const& identifier);

    QVector<SymbolTableEntryPtr> getSymbolTableEntries() const;

    bool addParentSymbolTable(const SymbolTableEntryPtr parent);

    SymbolTableEntryPtr getParentSymbolTable() const;

    QString getIdentifier() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

private:
    QHash<QString,int> SymbolTableIndices;
    QVector<SymbolTableEntryPtr> SymbolTableEntries;
    SymbolTableEntryPtr parentSymbolTable; // DO NOT TOUCH PARENT SYMBOL TABLE
};

typedef QSharedPointer<SymbolTable> SymbolTablePtr;

class Symbol : public SymbolTableEntry
{
public:
    Symbol();
    ~Symbol();

    virtual SimpleNode::ValueTypes getReturnType() const;
    virtual const ValueNode &visit() = 0;

    // SymbolTableEntry interface
public:
    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
};

typedef QSharedPointer<Symbol> SymbolPtr;

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(
            const QString &identifier,
            SimpleNode::ValueTypes VariableType = SimpleNode::Integer
            );
    ~VariableSymbol();

    const ValueNode &getAssignedValue() const;

    bool assignValue(const SimpleNode &NodeToAssign);

    SimpleNode::ValueTypes getVariableType() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

    // Symbol interface
public:
    SimpleNode::ValueTypes getReturnType() const;

private:
    ValueNode AssignedNode; // value...
    SimpleNode::ValueTypes VariableType; // FIXED -> test for ability to cast when assigned
};

typedef QSharedPointer<VariableSymbol> VariableSymbolPtr;

class FunctionSymbol : public Symbol
{
public:
    FunctionSymbol(QString &identifier,
            QVector<VariableNode> &FunctionParameters,
            SimpleNode::ValueTypes ReturnType = SimpleNode::Void
            );
    ~FunctionSymbol();

    void addFunctionExpressions(const QVector<SimpleNode> &FuncExpressions);
    void addFunctionReturnStatement(const SimpleNode &returnNode);

    ValueNode CallFunction(
            const QVector<SimpleNode> &FunctionArguments,
            SymbolTableEntryPtr CurrentSymbolTable
            );

    bool checkFunctionArguments(const QVector<VariableNode> &FunctionArguments) const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    virtual QString PrintToSymbolToString() const;
    virtual QString PrintSymbolType() const;

    // Symbol interface
public:
    SimpleNode::ValueTypes getReturnType() const;

private:
    SimpleNode::ValueTypes ReturnType;
    QVector<VariableNode> FunctionParameters;
    SymbolTable FunctionSymbolTable;
    QVector<SimpleNode> FunctionExpressions;
    SimpleNode FunctionReturnNode;
};

typedef QSharedPointer<FunctionSymbol> FunctionSymbolPtr;

#endif // SIMPLESYMBOLTABLE_H
