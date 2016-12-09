#ifndef SELECTIONCONTROLNODE_H
#define SELECTIONCONTROLNODE_H

#include "controlnode.h"

class SelectionControlNode : public ControlNode
{
public:
    SelectionControlNode();
    SelectionControlNode(const SelectionControlNode &ToCopy);
    virtual ~SelectionControlNode();

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    ControlNode::SpecificControlType getSpecificControlType() const = 0;
};

class IfNode : public SelectionControlNode
{
public:
    IfNode(std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> StatementBlock);
    IfNode(const IfNode &ToCopy);
    ~IfNode();

    const std::unique_ptr<SimpleNode> &getIfCondition() const;
    const std::unique_ptr<SimpleNode> &getIfStatementBlock() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> IfCondition;
    std::unique_ptr<SimpleNode> IfStatementBlock;
};

class ElseNode : public SelectionControlNode
{
public:
    ElseNode(std::unique_ptr<SimpleNode> StatementBlock);
    ElseNode(const ElseNode &ToCopy);
    ~ElseNode();

    const std::unique_ptr<SimpleNode> &getElseStatementBlock() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> ElseStatementBlock;
};

class SwitchNode : public SelectionControlNode
{
public:
    SwitchNode(std::unique_ptr<SimpleNode> Condition);
    SwitchNode(const SwitchNode &ToCopy);
    ~SwitchNode();

    const std::unique_ptr<SimpleNode> &getSwitchCondition() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> SwitchCondition;
//    std::vector<std::unique_ptr<SimpleNode>>
};

#endif // SELECTIONCONTROLNODE_H
