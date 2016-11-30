#include "operationnodes.h"

#include <QDebug>

#include "valuenode.h"

#include "simplestack.h"

OperationNode::OperationNode()
{
}

OperationNode::~OperationNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType OperationNode::getNodeType() const
{
    return Node::Operation;
}

Node::ValueTypes OperationNode::getReturnType() const
{
    return returnType;
}

std::unique_ptr<ValueNode> OperationNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    return DoOperation(StackToUse);
}

UnaryOperationNode::UnaryOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOPRightChild(std::move(rightChild))
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


BinaryOperationNode::BinaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOPLeftChild(std::move(leftChild)),
    BinaryOPRightChild(std::move(rightChild))
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

TernaryOperationNode::TernaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOPLeftChild(std::move(leftChild)),
    TernaryOPMidChild(std::move(midChild)),
    TernaryOPRightChild(std::move(rightChild))
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
