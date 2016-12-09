#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H

#include "simplenode.h"

class ExpressionNode : public NonTerminalNode
{
public:
    ExpressionNode();

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;
};

#endif // EXPRESSIONNODE_H
