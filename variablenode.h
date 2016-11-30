#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include <QString>
#include <QSharedPointer>
#include "variablesymbol.h"

class VariableNode : public SimpleNode
{
public:
    VariableNode(QSharedPointer<VariableSymbol> relatedVariableSymbol);
    ~VariableNode();
    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;

    std::unique_ptr<ValueNode> visit() const;

    QString printValue() const;
    virtual QString printNode() const;

    QString getVariableName() const;

    QSharedPointer<VariableSymbol> getRelatedVariableSymbol() const;

private:
    QSharedPointer<VariableSymbol> RelatedVariableSymbol;
};

#endif // VARIABLENODE_H
