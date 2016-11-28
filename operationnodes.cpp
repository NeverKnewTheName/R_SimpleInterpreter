#include "operationnodes.h"

OperationNode::OperationNode()
{
}

OperationNode::~OperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType OperationNode::getNodeType() const
{
    return SimpleNode::Operation;
}

SimpleNode::ValueTypes OperationNode::getReturnType() const
{
    return returnType;
}

const ValueNodeScopedPtr OperationNode::visit()
{
    return ValueNodeScopePtr( new ValueNode(*(DoOperation())));
}

UnaryOperationNode::UnaryOperationNode(const ValueNodeScopedPtr rightChild) :
    rightChild(rightChild)
{
}

UnaryOperationNode::~UnaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes UnaryOperationNode::getArityType() const
{
    return OperationNode::Unary;
}


BinaryOperationNode::BinaryOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr rightChild) :
    leftChild(leftChild),
    rightChild(rightChild)
{
}

BinaryOperationNode::~BinaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes BinaryOperationNode::getArityType() const
{
    return OperationNode::Binary;
}

TernaryOperationNode::TernaryOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild, const SimpleNodeScopedPtr rightChild) :
    leftChild(leftChild),
    midChild(midChild),
    rightChild(rightChild)
{
}

TernaryOperationNode::~TernaryOperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

OperationNode::ArityTypes TernaryOperationNode::getArityType() const
{
    return OperationNode::Ternary;
}
