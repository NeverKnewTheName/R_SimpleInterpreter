#ifndef ITERATIONCONTROLNODE_H
#define ITERATIONCONTROLNODE_H

#include "controlnode.h"

class IterationControlNode : public ControlNode
{
public:
    IterationControlNode(std::unique_ptr<SimpleNode> Statement);
    IterationControlNode(const IterationControlNode &ToCopy);
    virtual ~IterationControlNode();

    void replaceStatement(std::unique_ptr<SimpleNode> Statement);
    const std::unique_ptr<SimpleNode> &getIterationStatement() const;

    // ScopedControlNodeInterface
public:
    ControlNode::ControlType getControlType() const;
    virtual ControlNode::SpecificControlType getSpecificControlType() const = 0;

private:
    std::unique_ptr<SimpleNode> IterationStatement;
};

// // // // // // // // // // // // // // // // // // // // // //

class ForLoopNode : public IterationControlNode
{
public:
    ForLoopNode(
            std::unique_ptr<SimpleNode> Initialization,
            std::unique_ptr<SimpleNode> Condition,
            std::unique_ptr<SimpleNode> Update,
            std::unique_ptr<SimpleNode> Statement
            );
    ForLoopNode(const ForLoopNode &ToCopy);
    ~ForLoopNode();

    const std::unique_ptr<SimpleNode> &getForLoopInitialization() const;
    const std::unique_ptr<SimpleNode> &getForLoopCondition() const;
    const std::unique_ptr<SimpleNode> &getForLoopUpdate() const;

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    Node::ValueTypes getReturnType() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

private:
    std::unique_ptr<SimpleNode> ForLoopInitialization;
    std::unique_ptr<SimpleNode> ForLoopCondition;
    std::unique_ptr<SimpleNode> ForLoopUpdate;
};

// // // // // // // // // // // // // // // // // // // // // //

class WhileLoopNode : public IterationControlNode
{
public:
    WhileLoopNode(
            std::unique_ptr<SimpleNode> Condition,
            std::unique_ptr<SimpleNode> Statement
            );
    WhileLoopNode(const WhileLoopNode &ToCopy);
    ~WhileLoopNode();

    const std::unique_ptr<SimpleNode> &getWhileCondition();

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    Node::ValueTypes getReturnType() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    virtual ControlNode::SpecificControlType getSpecificControlType() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;

private:
    std::unique_ptr<SimpleNode> WhileLoopCondition;
};

// // // // // // // // // // // // // // // // // // // // // //

class DoWhileLoopNode : public WhileLoopNode
{
public:
    DoWhileLoopNode(
            std::unique_ptr<SimpleNode> Condition,
            std::unique_ptr<SimpleNode> Statement
            );
    ~DoWhileLoopNode();

    // SimpleNode interface
public:
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ControlNode interface
public:
    ControlNode::SpecificControlType getSpecificControlType() const;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // ITERATIONCONTROLNODE_H
