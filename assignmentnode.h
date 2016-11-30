#ifndef ASSIGNMENTNODE_H
#define ASSIGNMENTNODE_H

#include "simplenode.h"

class VariableNode;

class AssignmentNode : public SimpleNode
{
public:
//    AssignmentNode();
    AssignmentNode(std::unique_ptr<VariableNode> VariableToAssign, std::unique_ptr<SimpleNode> ValueToAssign);
    virtual ~AssignmentNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit() const;

private:
    std::unique_ptr<VariableNode> VariableToAssign;
    std::unique_ptr<SimpleNode> ValueToAssign;
};

#endif // ASSIGNMENTNODE_H
