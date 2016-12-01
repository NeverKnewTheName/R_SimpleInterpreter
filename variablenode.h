#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include <QString>
#include <QSharedPointer>
#include "variablesymbol.h"

class VariableNode : public SimpleNode
{
public:
    VariableNode(QSharedPointer<ValueSymbol> relatedVariableSymbol);
    ~VariableNode();
    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    QString printValue() const;
    virtual QString printNode() const;

    QString getVariableName() const;

    QSharedPointer<ValueSymbol> getRelatedVariableSymbol() const;

private:
    QSharedPointer<ValueSymbol> RelatedVariableSymbol;
};

#endif // VARIABLENODE_H
