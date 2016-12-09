#include "operationnodes.h"

#include <QDebug>

#include "valuenode.h"

#include "simplestack.h"

#include "astvisualizer.h"

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

UnaryOperationNode::UnaryOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOPRightChild(std::move(rightChild))
{
}

UnaryOperationNode::UnaryOperationNode(const UnaryOperationNode &ToCopy) :
    UnaryOPRightChild(ToCopy.UnaryOPRightChild->deepCopy()),
    implicitCastRightChild(ToCopy.implicitCastRightChild)
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

const std::unique_ptr<SimpleNode> &UnaryOperationNode::getUnaryOPRightChild() const
{
    return UnaryOPRightChild;
}

Node::ValueTypes UnaryOperationNode::getImplicitCastRightChild() const
{
    return implicitCastRightChild;
}


BinaryOperationNode::BinaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOPLeftChild(std::move(leftChild)),
    BinaryOPRightChild(std::move(rightChild))
{
}

BinaryOperationNode::BinaryOperationNode(const BinaryOperationNode &ToCopy) :
    BinaryOPLeftChild(std::unique_ptr<SimpleNode>(ToCopy.BinaryOPLeftChild->deepCopy())),
    BinaryOPRightChild(std::unique_ptr<SimpleNode>(ToCopy.BinaryOPRightChild->deepCopy())),
    implicitCastLeftChild(ToCopy.implicitCastLeftChild),
    implicitCastRightChild(ToCopy.implicitCastRightChild)
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

const std::unique_ptr<SimpleNode> &BinaryOperationNode::getBinaryOPLeftChild() const
{
    return BinaryOPLeftChild;
}

const std::unique_ptr<SimpleNode> &BinaryOperationNode::getBinaryOPRightChild() const
{
    return BinaryOPRightChild;
}

Node::ValueTypes BinaryOperationNode::getImplicitCastLeftChild() const
{
    return implicitCastLeftChild;
}

Node::ValueTypes BinaryOperationNode::getImplicitCastRightChild() const
{
    return implicitCastRightChild;
}

TernaryOperationNode::TernaryOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOPLeftChild(std::move(leftChild)),
    TernaryOPMidChild(std::move(midChild)),
    TernaryOPRightChild(std::move(rightChild))
{
}

TernaryOperationNode::TernaryOperationNode(const TernaryOperationNode &ToCopy) :
    TernaryOPLeftChild(ToCopy.TernaryOPLeftChild->deepCopy()),
    TernaryOPMidChild(ToCopy.TernaryOPMidChild->deepCopy()),
    TernaryOPRightChild(ToCopy.TernaryOPRightChild->deepCopy()),
    implicitCastLeftChild(ToCopy.implicitCastLeftChild),
    implicitCastMidChild(ToCopy.implicitCastMidChild),
    implicitCastRightChild(ToCopy.implicitCastRightChild)
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

/*
 * When parsing the tree temp variables will store immediate values
 *
 * as long as the walker does not catch a node that requires the previous value, it will keep it and simply use another temporary storage
 * -> might be an array with a reasonable size (3 for ternary operations?? what about nesting?)
 */

const std::unique_ptr<SimpleNode> &TernaryOperationNode::getTernaryOPLeftChild() const
{
    return TernaryOPLeftChild;
}

const std::unique_ptr<SimpleNode> &TernaryOperationNode::getTernaryOPMidChild() const
{
    return TernaryOPMidChild;
}

const std::unique_ptr<SimpleNode> &TernaryOperationNode::getTernaryOPRightChild() const
{
    return TernaryOPRightChild;
}

Node::ValueTypes TernaryOperationNode::getImplicitCastLeftChild() const
{
    return implicitCastLeftChild;
}

Node::ValueTypes TernaryOperationNode::getImplicitCastMidChild() const
{
    return implicitCastMidChild;
}

Node::ValueTypes TernaryOperationNode::getImplicitCastRightChild() const
{
    return implicitCastRightChild;
}
