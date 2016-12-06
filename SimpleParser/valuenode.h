#ifndef VALUENODE_H
#define VALUENODE_H

#include "simplenode.h"

class ValueNode : public SimpleNode
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
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
//    uint8_t FlatCompileOPCode(int &curStackOffset) const;

    std::unique_ptr<SimpleNode> deepCopy() const;

private:
    Node::ValueTypes valueType;
    QVariant value;
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
//    ASTNode *VisualizeNode(ASTNode *parentNode) const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    // ValueNode interface
public:
    const QVariant getValue() const;
};


#endif // VALUENODE_H
