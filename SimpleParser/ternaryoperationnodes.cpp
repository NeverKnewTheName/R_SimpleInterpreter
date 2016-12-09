#include "ternaryoperationnodes.h"

#include "valuenode.h"
#include "simplestack.h"

#include "simplenodevisitor.h"

TernaryArithmeticOperationNode::TernaryArithmeticOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOperationNode(std::move(leftChild), std::move(midChild), std::move(rightChild))
{

}

OperationNode::OperationTypes TernaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TernaryLogicalOperationNode::TernaryLogicalOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOperationNode(std::move(leftChild), std::move(midChild), std::move(rightChild))
{

}

OperationNode::OperationTypes TernaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

ConditionalNode::ConditionalNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryLogicalOperationNode(std::move(leftChild), std::move(midChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = TernaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeMChild = TernaryOPMidChild->getReturnType();
    Node::ValueTypes returnTypeRChild = TernaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    if( ( returnTypeLChild == Node::Bool ) ||
            ( returnTypeLChild == Node::Integer ) ||
            ( returnTypeLChild == Node::Double ) )
    {
        implicitCastLeftChild = Node::Bool;
        switch(returnTypeMChild)
        {
        case Node::Integer:
            if(returnTypeRChild == Node::Double)
            {
                implicitCastMidChild = Node::Double;
                implicitCastRightChild = Node::Double;
                returnType = Node::Double;
            }
            else if( returnTypeRChild == Node::Bool )
            {
                implicitCastMidChild = Node::Bool;
                implicitCastRightChild = Node::Bool;
                returnType = Node::Bool;
            }
            else if( returnTypeRChild != Node::String )
            {
                implicitCastMidChild = Node::Integer;
                implicitCastRightChild = Node::Integer;
                returnType = Node::Integer;
            }
            break;
        case Node::Double:
            if( returnTypeRChild == Node::Bool )
            {
                implicitCastMidChild = Node::Bool;
                implicitCastRightChild = Node::Bool;
                returnType = Node::Bool;
            }
            else if( returnTypeRChild != Node::String )
            {
                implicitCastMidChild = Node::Double;
                implicitCastRightChild = Node::Double;
                returnType = Node::Double; // Implicitly casts Integer to Double
            }
            break;
        case Node::Bool:
            if( returnTypeRChild != Node::String )
            {
                implicitCastMidChild = Node::Bool;
                implicitCastRightChild = Node::Bool;
                returnType = Node::Bool; // Implicitly casts Integer and Double to Bool
            }
            break;
        case Node::String: // if one return ChildNode is a String, the other node must also be a String
            if( returnTypeRChild == Node::String )
            {
                implicitCastMidChild = Node::String;
                implicitCastRightChild = Node::String;
                returnType = Node::String;
            }
            break;
        }
    }
}

OperationNode::Operation ConditionalNode::getOp() const
{
    return OperationNode::Conditional;
}

OperationNode::Associativity ConditionalNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence ConditionalNode::getPrecedence() const
{
    return OperationNode::ConditionalPrec;
}

QString ConditionalNode::printValue() const
{
    return QString("? :");
}

QString ConditionalNode::printNode() const
{
    QString NodeType = "ConditionalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> ConditionalNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ConditionalNode(*this));
}

TernaryBitwiseOperationNode::TernaryBitwiseOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild) :
    TernaryOperationNode(std::move(leftChild), std::move(midChild), std::move(rightChild))
{

}

OperationNode::OperationTypes TernaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}


void ConditionalNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ConditionalNode>(new ConditionalNode(*this)));
}
