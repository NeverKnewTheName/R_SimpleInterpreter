#ifndef CONTROLNODE_H
#define CONTROLNODE_H

#include <QString>
#include <QSharedPointer>

#include "simplenode.h"

class SimpleSymbolTable;

class ControlNode : public NonTerminalNode
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
    virtual Node::NodeType getNodeType() const;

};

// // // // // // // // // // // // // // // //

class SelectionControlNode : public ControlNode
{
public:
    SelectionControlNode();
    virtual ~SelectionControlNode();

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;

    //Must redirect call to ScopeControlNode... copy shared scope symbol table
    virtual std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::ControlType getControlType() const;
    ControlNode::SpecificControlType getSpecificControlType() const;
};

// // // // // // // // // // // // // // // //


// // // // // // // // // // // // // // // //


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
    virtual QString printValue() const;
    virtual QString printNode() const;
    virtual std::unique_ptr<SimpleNode> deepCopy() const;
};

// // // // // // // // // // // // // // // //


#endif // CONTROLNODE_H
