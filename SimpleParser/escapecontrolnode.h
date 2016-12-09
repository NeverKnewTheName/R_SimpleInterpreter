#ifndef ESCAPECONTROLNODE_H
#define ESCAPECONTROLNODE_H

#include "controlnode.h"

class EscapeControlNode : public ControlNode
{
public:
    EscapeControlNode();
    virtual ~EscapeControlNode();

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;

    // SimpleNode interface
public:
    virtual Node::ValueTypes getReturnType() const;
};

class BreakNode : public EscapeControlNode
{
public:
    BreakNode();
    BreakNode(const BreakNode &ToCopy);
    ~BreakNode();

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;
};

class ContinueNode : public EscapeControlNode
{
public:
    ContinueNode();
    ContinueNode(const ContinueNode &ToCopy);
    ~ContinueNode();

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;
};

class ReturnNode : public EscapeControlNode
{
public:
    ReturnNode(std::unique_ptr<SimpleNode> ReturnExpression);
    ReturnNode(const ReturnNode &ToCopy);
    ~ReturnNode();

    const std::unique_ptr<SimpleNode> &getReturnExpression() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    Node::ValueTypes getReturnType() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> ReturnExpression;
};

#endif // ESCAPECONTROLNODE_H
