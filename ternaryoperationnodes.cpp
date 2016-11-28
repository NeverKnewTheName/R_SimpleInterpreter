#include "ternaryoperationnodes.h"

TernaryArithmeticOperationNode::TernaryArithmeticOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild, const SimpleNodeScopedPtr rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TernaryLogicalOperationNode::TernaryLogicalOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild, const SimpleNodeScopedPtr rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

ConditionalNode::ConditionalNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild, const SimpleNodeScopedPtr rightChild) :
    TernaryLogicalOperationNode(leftChild, midChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeMChild = midChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    if( ( returnTypeLChild == ValueNode::Bool ) ||
            ( returnTypeLChild == ValueNode::Integer ) ||
            ( returnTypeLChild == ValueNode::Double ) )
    {
        implicitCastLeftChild = ValueNode::Bool;
        switch(returnTypeMChild)
        {
        case ValueNode::Integer:
            if(returnTypeRChild == ValueNode::Double)
            {
                implicitCastMidChild = ValueNode::Double;
                implicitCastRightChild = ValueNode::Double;
                returnType = ValueNode::Double;
            }
            else if( returnTypeRChild == ValueNode::Bool )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool;
            }
            else if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Integer;
                implicitCastRightChild = ValueNode::Integer;
                returnType = ValueNode::Integer;
            }
            break;
        case ValueNode::Double:
            if( returnTypeRChild == ValueNode::Bool )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool;
            }
            else if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Double;
                implicitCastRightChild = ValueNode::Double;
                returnType = ValueNode::Double; // Implicitly casts Integer to Double
            }
            break;
        case ValueNode::Bool:
            if( returnTypeRChild != ValueNode::String )
            {
                implicitCastMidChild = ValueNode::Bool;
                implicitCastRightChild = ValueNode::Bool;
                returnType = ValueNode::Bool; // Implicitly casts Integer and Double to Bool
            }
            break;
        case ValueNode::String: // if one return ChildNode is a String, the other node must also be a String
            if( returnTypeRChild == ValueNode::String )
            {
                implicitCastMidChild = ValueNode::String;
                implicitCastRightChild = ValueNode::String;
                returnType = ValueNode::String;
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

const ValueNodeScopedPtr ConditionalNode::DoOperation()
{
    ValueNodeScopedPtr value1;
    value1->swap(leftChild->visit());
    ValueNodeScopedPtr value2;
    value2->swap(midChild->visit());
    ValueNodeScopedPtr value3;
    value3->swap(rightChild->visit());

    bool IsTrue = value1->getValue().value<bool>();

    switch(implicitCastMidChild)
    {
    case SimpleNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<double>() : value3->getValue().value<double>() ));
        }
        else if(implicitCastRightChild == SimpleNode::Bool)
        {
            return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<bool>() : value3->getValue().value<bool>() ));
        }
        else
        {
            return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<int>() : value3->getValue().value<int>() ));
        }
        break;
    case SimpleNode::Double:
        if(implicitCastRightChild == SimpleNode::Bool)
        {
            return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<bool>() : value3->getValue().value<bool>() ));
        }
        else
        {
            return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<double>() : value3->getValue().value<double>() ));
        }
        break;
    case SimpleNode::Bool:
        return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<bool>() : value3->getValue().value<bool>() ));
        break;
    case SimpleNode::String:
        return ValueNodeScopedPtr(new ValueNode((IsTrue) ? value2->getValue().value<QString>() : value3->getValue().value<QString>() ));
        break;
    default:
        return ValueNodeScopedPtr(new ValueNode());
    }

}

QString ConditionalNode::printValue() const
{
    return QString("?:");
}

QString ConditionalNode::printNode() const
{
    QString NodeType = "ConditionalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

TernaryBitwiseOperationNode::TernaryBitwiseOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild, const SimpleNodeScopedPtr rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{

}

OperationNode::OperationTypes TernaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}
