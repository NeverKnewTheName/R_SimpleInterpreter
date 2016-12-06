#ifndef ITERATIONCONTROLNODE_H
#define ITERATIONCONTROLNODE_H

#include "controlnode.h"

class IterationControlNode : public ScopedControlNode
{
public:
    IterationControlNode();
    virtual ~IterationControlNode();

    virtual void executeLoop(QSharedPointer<SimpleStack> StackToUse) const = 0;

    // SimpleNode interface
public:
    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;

    //Must redirect call to ScopeControlNode... copy shared scope symbol table
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // ScopedControlNodeInterface
public:
    ControlNode::ControlType getControlType() const;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;
};

// // // // // // // // // // // // // // // // // // // // // //

class ForLoopNode : public IterationControlNode
{
public:
    ForLoopNode(std::unique_ptr<SimpleNode> Initialization,
            std::unique_ptr<SimpleNode> Condition,
            std::unique_ptr<SimpleNode> Update,
            std::vector<std::unique_ptr<SimpleNode> > &Expressions
            );
    ForLoopNode(const ForLoopNode &ToCopy);
    ~ForLoopNode();

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    //uint8_t FlatCompileOPCode(int &curStackOffset) const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> ForLoopInitialization;
    std::unique_ptr<SimpleNode> ForLoopCondition;
    std::unique_ptr<SimpleNode> ForLoopUpdate;
    std::vector<std::unique_ptr<SimpleNode>> ForLoopExpressions;

    // IterationControlNode interface
public:
    void executeLoop(QSharedPointer<SimpleStack> StackToUse) const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;
};

// // // // // // // // // // // // // // // // // // // // // //

class WhileLoopNode : public IterationControlNode
{
public:
    WhileLoopNode(std::unique_ptr<SimpleNode> Condition,
            std::vector<std::unique_ptr<SimpleNode> > &Expressions
            );
    WhileLoopNode(const WhileLoopNode &ToCopy);
    ~WhileLoopNode();

    // SimpleNode interface
public:
    Node::ValueTypes getReturnType() const;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    //uint8_t FlatCompileOPCode(int &curStackOffset) const;

    // ControlNode interface
public:
    virtual ControlNode::SpecificControlType getSpecificControlType() const;

private:
    std::unique_ptr<SimpleNode> WhileLoopCondition;
    std::vector<std::unique_ptr<SimpleNode>> WhileLoopExpressions;

    // IterationControlNode interface
public:
    void executeLoop(QSharedPointer<SimpleStack> StackToUse) const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;
};

// // // // // // // // // // // // // // // // // // // // // //

class DoWhileLoopNode : public WhileLoopNode
{
public:
    DoWhileLoopNode(
            std::unique_ptr<SimpleNode> Condition,
            std::vector<std::unique_ptr<SimpleNode> > &Expressions
            );
    ~DoWhileLoopNode();

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    //uint8_t FlatCompileOPCode(int &curStackOffset) const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
};

#endif // ITERATIONCONTROLNODE_H
