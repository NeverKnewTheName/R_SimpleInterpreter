#ifndef VARIABLESYMBOL_H
#define VARIABLESYMBOL_H

#include <QString>
#include "symbol.h"
#include "valuenode.h"


class VariableSymbol : public Symbol
{
public:
    VariableSymbol(
            const QString &identifier,
            Node::ValueTypes VariableType = Node::Integer
            );
    ~VariableSymbol();

    std::unique_ptr<ValueNode> getAssignedValue() const;

    bool assignValue(const SimpleNode &NodeToAssign);

    Node::ValueTypes getVariableType() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    Node::ValueTypes getReturnType() const;

private:
    ValueNode AssignedNode; // value...
    Node::ValueTypes VariableType; // FIXED -> test for ability to cast when assigned
};

#endif // VARIABLESYMBOL_H
