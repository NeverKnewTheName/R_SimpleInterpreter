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

class IfNode : public SelectionControlNode
{
public:
    IfNode(std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> StatementBlock);
    IfNode(const IfNode &ToCopy);
    ~IfNode();

    const std::unique_ptr<SimpleNode> &getIfCondition() const;
    const std::unique_ptr<SimpleNode> &getIfStatementBlock() const;
    void addElse(std::unique_ptr<ElseNode> ElseNode);

    const std::unique_ptr<SimpleNode> &getIfElseNode() const;

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
    std::unique_ptr<SimpleNode> IfElseNode;
};

class SwitchLabel : public NonTerminalNode
{
public:
    typedef enum _SwitchLabelType
    {
        CaseLabel,
        DefaultLable
    }SwitchLabelType;

    SwitchLabel(std::unique_ptr<SimpleNode> Statement);
    SwitchLabel(const SwitchLabel &ToCopy);
    virtual ~SwitchLabel();
    const std::unique_ptr<SimpleNode> &getSwitchLabelStatement() const;

    virtual SwitchLabelType getSwitchLabelType() const = 0;

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;

private:
    Node::ValueTypes returnType;
    std::unique_ptr<SimpleNode> SwitchLabelStatement;
};

class CaseNode : public SwitchLabel
{
public:
    CaseNode(std::unique_ptr<ValueNode> Argument, std::unique_ptr<SimpleNode> Statement);
    CaseNode(const CaseNode &ToCopy);
    ~CaseNode();

    const std::unique_ptr<ValueNode> &getCaseArgument() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // SwitchLabel interface
public:
    SwitchLabelType getSwitchLabelType() const;

private:
    std::unique_ptr<ValueNode> CaseArgument;
};

class DefaultNode : public SwitchLabel
{
public:
    DefaultNode(std::unique_ptr<SimpleNode> Statement);
    DefaultNode(const DefaultNode &ToCopy);
    ~DefaultNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SwitchLabel interface
public:
    SwitchLabelType getSwitchLabelType() const;
};

class SwitchNode : public SelectionControlNode
{
public:
    SwitchNode(std::unique_ptr<SimpleNode> Condition);
    SwitchNode(const SwitchNode &ToCopy);
    ~SwitchNode();

    const std::unique_ptr<SimpleNode> &getSwitchCondition() const;

    void addSwitchLabel(std::unique_ptr<SwitchLabel> SwitchLabel);

    const std::vector<std::unique_ptr<SwitchLabel> > &getSwitchLabels() const;

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
    Node::ValueTypes returnType;
    std::unique_ptr<SimpleNode> SwitchCondition;
    std::vector<std::unique_ptr<SwitchLabel>> SwitchLabels;
};

#endif // SELECTIONCONTROLNODE_H
