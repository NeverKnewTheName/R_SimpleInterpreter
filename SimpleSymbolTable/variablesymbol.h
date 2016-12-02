#ifndef VARIABLESYMBOL_H
#define VARIABLESYMBOL_H

#include <vector>
#include <QString>
#include "simplesymbol.h"
#include "valuenode.h"


class ValueSymbol : public SimpleSymbol
{
public:
    ValueSymbol(
            const QString &identifier,
            const Node::ValueTypes ValueType = Node::Integer
            );
    virtual ~ValueSymbol();

    virtual std::unique_ptr<ValueNode> getValue(QSharedPointer<SimpleStack> StackToUse) const = 0;

    // SimpleSymbolTableEntry interface
public:
    virtual SymbolTableEntryType getType() const = 0;
    virtual QString PrintToSymbolToString() const = 0;
    virtual QString PrintSymbolType() const = 0;

    // SimpleSymbol interface
public:
    Node::ValueTypes getReturnType() const;

private:
    Node::ValueTypes ValueType;
};

class VariableSymbol : public ValueSymbol
{
public:
    VariableSymbol(
            const QString &identifier,
            const Node::ValueTypes VariableType = Node::Integer
            );
    ~VariableSymbol();

    std::unique_ptr<ValueNode> getAssignedValue(QSharedPointer<SimpleStack> StackToUse) const;

    bool assignValue(std::unique_ptr<SimpleNode> NodeToAssign,QSharedPointer<SimpleStack> StackToUse) const;

    Node::ValueTypes getVariableType() const;

    bool VarEnterScope(QSharedPointer<SimpleStack> StackToUse);
    bool VarExitScope(QSharedPointer<SimpleStack> StackToUse);

    bool addScope();
    bool removeScope();

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // ValueSymbol interface
public:
    std::unique_ptr<ValueNode> getValue(QSharedPointer<SimpleStack> StackToUse) const;

private:
    std::vector<unsigned int> AddressPerScope; //Currently is only the stack index
    unsigned int CurrentScope; //IS THIS REALLY NEEDED??? CurrentScope will actually always be the last one...
};

/**
 * @brief Constant values do not need scoping, since their value cannot be changed
 * THUS regardless of scope the value remains the same
 */
class ConstantValueSymbol : public ValueSymbol
{
public:
    ConstantValueSymbol(
            const QString &identifier,
            ValueNode value
            );
    ~ConstantValueSymbol();

    // SimpleSymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // ValueSymbol interface
public:
    std::unique_ptr<ValueNode> getValue(QSharedPointer<SimpleStack> StackToUse) const;

private:
    ValueNode ConstVal;
};

#endif // VARIABLESYMBOL_H
