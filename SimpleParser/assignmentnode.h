#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "simplenode.h"

class VariableNode;

class AssignmentNode : public NonTerminalNode
{
public:
//    AssignmentNode();
    AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign);
    AssignmentNode(const AssignmentNode &ToCopy);
    virtual ~AssignmentNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    const std::unique_ptr<VariableNode> &getVariableToAssign() const;

    const std::unique_ptr<SimpleNode> &getValueToAssign() const;

private:
    std::unique_ptr<VariableNode> VariableToAssign;
    std::unique_ptr<SimpleNode> ValueToAssign;
    Node::ValueTypes ReturnType;

    // SimpleNode interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // ASSIGNMENTNODE_H
