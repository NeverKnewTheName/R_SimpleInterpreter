#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QVariant>
#include <QString>

#include <QSharedPointer>
#include <QScopedPointer>

#include "simpleast.h"

class SimpleNode;
class ValueNode;
class FunctionNode;

class VariableNode;

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
    QString getIdentifier() const;
protected:
    QString identifier;
    bool isAssigned;
};

typedef QSharedPointer<SymbolTableEntry> SymbolTableEntryPtr;
typedef QScopedPointer<SymbolTableEntry> SymbolTableEntryScopedPtr;

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
typedef QScopedPointer<SymbolTable> SymbolTableScopedPtr;

class Symbol : public SymbolTableEntry
{
public:
    Symbol();
    ~Symbol();

    virtual SimpleNode::ValueTypes getReturnType() const;
//    virtual const ValueNode &visit() = 0;

    // SymbolTableEntry interface
public:
    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;
};

typedef QSharedPointer<Symbol> SymbolPtr;
typedef QScopedPointer<Symbol> SymbolScopedPtr;

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(
            const QString &identifier,
            SimpleNode::ValueTypes VariableType = SimpleNode::Integer
            );
    ~VariableSymbol();

    ValueNodeScopedPtr getAssignedValue() const;

    bool assignValue(const SimpleNode &NodeToAssign);

    SimpleNode::ValueTypes getVariableType() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    SimpleNode::ValueTypes getReturnType() const;

private:
    ValueNode AssignedNode; // value...
    SimpleNode::ValueTypes VariableType; // FIXED -> test for ability to cast when assigned
};

typedef QSharedPointer<VariableSymbol> VariableSymbolPtr;
typedef QScopedPointer<VariableSymbol> VariableSymbolScopedPtr;

class FunctionSymbol : public Symbol
{
public:
    FunctionSymbol(QString &identifier,
            QVector<VariableSymbolPtr> &&functionParameters,
            SimpleNode::ValueTypes ReturnType = SimpleNode::Void
            );
    ~FunctionSymbol();

    void addFunctionExpressions(QVector<SimpleNodeScopedPtr> FuncExpressions);
    void addFunctionReturnStatement(SimpleNodeScopedPtr returnNode);

    ValueNode CallFunction(
            QVector<SimpleNodeScopedPtr> FunctionArguments,
            SymbolTableEntryPtr CurrentSymbolTable
            );

    bool checkFunctionArguments(const QVector<SimpleNodeScopedPtr> &FunctionArguments) const;

    SymbolTable &getFunctionSymbolTable() const;
    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    SimpleNode::ValueTypes getReturnType() const;


private:
    SimpleNode::ValueTypes ReturnType;
    QVector<QSharedPointer<VariableNode>> FunctionParameters;
    SymbolTable FunctionSymbolTable;
    QVector<SimpleNodeScopedPtr> FunctionExpressions;
    SimpleNodeScopedPtr FunctionReturnNode;
};

typedef QSharedPointer<FunctionSymbol> FunctionSymbolPtr;
typedef QScopedPointer<FunctionSymbol> FunctionSymbolScopedPtr;

#endif // SIMPLESYMBOLTABLE_H
