#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include "simplenode.h"

class StatementNode : public NonTerminalNode
{
public:
    StatementNode(std::unique_ptr<SimpleNode> Statement);
    StatementNode(const StatementNode &ToCopy);
    ~StatementNode();

    const std::unique_ptr<SimpleNode> &getStatement() const;

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

private:
    std::unique_ptr<SimpleNode> Statement;
};

#endif // STATEMENTNODE_H
