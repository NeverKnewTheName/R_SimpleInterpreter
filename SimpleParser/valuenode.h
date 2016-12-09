#ifndef VALUENODE_H
#define VALUENODE_H

#include "simplenode.h"

class ValueNode : public TerminalNode
{
public:

    ValueNode();
    ValueNode(const ValueNode &valueNodeToCopy);
    ValueNode(ValueNode &&valueNodeToMove);
    ValueNode(const int value);
    ValueNode(const double value);
    ValueNode(const bool value);
    ValueNode(QString const &value);
    virtual ~ValueNode();

    ValueNode &operator=(const ValueNode &value);
    ValueNode &operator=(ValueNode &&value);

    virtual const QVariant getValue() const;

    // SimpleNode interface
public:
    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;
    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual std::unique_ptr<SimpleNode> deepCopy() const;

private:
    Node::ValueTypes valueType;
    QVariant value;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

class VoidValueNode : public ValueNode
{
public:
    VoidValueNode();
    ~VoidValueNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

    // ValueNode interface
public:
    const QVariant getValue() const;


    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};


#endif // VALUENODE_H
