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

UnaryOperationNode::UnaryOperationNode(SimpleNodeScopedPtr rightChild) :
    rightChild(std::move(rightChild))
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


BinaryOperationNode::BinaryOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr rightChild) :
    leftChild(std::move(leftChild)),
    rightChild(std::move(rightChild))
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

TernaryOperationNode::TernaryOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr midChild, SimpleNodeScopedPtr rightChild) :
    leftChild(std::move(leftChild)),
    midChild(std::move(midChild)),
    rightChild(std::move(rightChild))
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
