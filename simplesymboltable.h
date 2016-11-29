#ifndef SIMPLESYMBOLTABLE_H
#define SIMPLESYMBOLTABLE_H

#include <QHash>
#include <QVariant>
#include <QString>

#include <QSharedPointer>

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
typedef std::unique_ptr<SymbolTableEntry> SymbolTableEntryUniquePtr;

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
typedef std::unique_ptr<SymbolTable> SymbolTableUniquePtr;

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
typedef std::unique_ptr<Symbol> SymbolUniquePtr;

class VariableSymbol : public Symbol
{
public:
    VariableSymbol(
            const QString &identifier,
            SimpleNode::ValueTypes VariableType = SimpleNode::Integer
            );
    ~VariableSymbol();

    ValueNodeUniquePtr getAssignedValue() const;

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
typedef std::unique_ptr<VariableSymbol> VariableSymbolUniquePtr;

class FunctionSymbol : public Symbol
{
public:
    FunctionSymbol(QString &identifier,
            QVector<VariableSymbolPtr> &&functionParameters,
            SimpleNode::ValueTypes ReturnType = SimpleNode::Void
            );
    ~FunctionSymbol();

    void addFunctionExpressions(QVector<SimpleNodeUniquePtr> &FuncExpressions);
    void addFunctionReturnStatement(SimpleNodeUniquePtr returnNode);

    ValueNode CallFunction(
            QVector<SimpleNodeUniquePtr> &FunctionArguments,
            SymbolTableEntryPtr CurrentSymbolTable
            );

    bool checkFunctionArguments(const QVector<SimpleNodeUniquePtr> &FunctionArguments) const;

    SymbolTablePtr getFunctionSymbolTable() const;
    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    SimpleNode::ValueTypes getReturnType() const;

private:
    SymbolTablePtr FunctionSymbolTable;
    SimpleNode::ValueTypes ReturnType;
    QVector<QSharedPointer<VariableNode>> FunctionParameters;
    QVector<SimpleNodeUniquePtr> FunctionExpressions;
    SimpleNodeUniquePtr FunctionReturnNode;
};

typedef QSharedPointer<FunctionSymbol> FunctionSymbolPtr;
typedef std::unique_ptr<FunctionSymbol> FunctionSymbolUniquePtr;

#endif // SIMPLESYMBOLTABLE_H
