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
    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const = 0;
    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;

    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const = 0;
};

// // // // // // // // // // // // // // // //

class ScopedControlNode : public ControlNode
{
public:
    ScopedControlNode();
    ScopedControlNode(const ScopedControlNode &ToCopy);
    virtual ~ScopedControlNode();

    const QSharedPointer<SimpleSymbolTable> &getScopedControlNodeSymbolTable() const;

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

    //Must copy shared pointer to ScopeSymbolTable
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // ControlNode interface
public:
    virtual ControlNode::ControlType getControlType() const = 0;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;
    static unsigned int ScopeCntr;
};

// // // // // // // // // // // // // // // //

class SelectionControlNode : public ScopedControlNode
{
public:
    SelectionControlNode();
    virtual ~SelectionControlNode();

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;

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
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;
};

// // // // // // // // // // // // // // // //


#endif // CONTROLNODE_H
