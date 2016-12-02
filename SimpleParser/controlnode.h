#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include <QString>
#include <QSharedPointer>

#include "simplenode.h"

class SimpleSymbolTable;

class ControlNode : public SimpleNode
{
public:
    typedef enum _ControlType
    {
        CONDITIONAL,
        ITERATION,
        ESCAPE,
        ERRORControlType
    }ControlType;
    typedef enum _SpecificControlType
    {
        IF,
        IFELSE,
        SWITCH,
        FOR,
        WHILE,
        DOWHILE,
        RETURN,
        BREAK,
        CONTINUE,
        ERRORSpecificControlType
    }SpecificControlType;

    ControlNode();
    virtual ~ControlNode();

    virtual ControlNode::ControlType getControlType() const = 0;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;

    static QString printControlTypeToString(ControlNode::ControlType controlType);
    static QString printSpecificControlTypeToString(ControlNode::SpecificControlType SpecificControlType);

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const = 0;
    QString printValue() const = 0;
    QString printNode() const = 0;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;
};

// // // // // // // // // // // // // // // //

class ScopedControlNode : public ControlNode
{
public:
    ScopedControlNode();
    virtual ~ScopedControlNode();

    QSharedPointer<SimpleSymbolTable> &getScopedControlNodeSymbolTable() const;

protected:
    bool EnterScope(QSharedPointer<SimpleStack> StackToUse) const;
    bool ExitScope(QSharedPointer<SimpleStack> StackToUse) const;

private:
    QSharedPointer<SimpleSymbolTable> ScopeSymbolTable;

    // SimpleNode interface
public:
    virtual Node::ValueTypes getReturnType() const = 0;
    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;

    // ControlNode interface
public:
    virtual ControlNode::ControlType getControlType() const = 0;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;
};

// // // // // // // // // // // // // // // //

class SelectionControlNode : public ScopedControlNode
{
public:
    SelectionControlNode();
    virtual ~SelectionControlNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    ControlNode::SpecificControlType getSpecificControlType() const;
};

// // // // // // // // // // // // // // // //


class IterationControlNode : public ScopedControlNode
{
public:
    IterationControlNode();
    virtual ~IterationControlNode();
    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    ControlNode::SpecificControlType getSpecificControlType() const;
};

// // // // // // // // // // // // // // // //


class EscapeControlNode : public ControlNode
{
public:
    EscapeControlNode();
    virtual ~EscapeControlNode();

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    ControlNode::SpecificControlType getSpecificControlType() const;
};

// // // // // // // // // // // // // // // //


#endif // CONTROLNODE_H
