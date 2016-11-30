#include "unaryoperationnodes.h"

#include "simplenode.h"
#include "valuenode.h"

#include "variablenode.h"
#include "programnode.h"


UnaryArithmeticOperationNode::UnaryArithmeticOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TypeCastNode::TypeCastNode(std::unique_ptr<SimpleNode> rightChild, Node::ValueTypes typeToCastTo) :
    UnaryArithmeticOperationNode(std::move(rightChild)),
    typeToCastTo(typeToCastTo)
{
    returnType = UnaryOPRightChild->getReturnType();

    switch(returnType)
    {
    case Node::Integer:
    case Node::Double:
    case Node::Bool:
        if(typeToCastTo != Node::ErrorType )
        {
            implicitCastRightChild = typeToCastTo;
            returnType = typeToCastTo;
        }
        else
        {
            implicitCastRightChild = Node::ErrorType;
            returnType = Node::ErrorType;
        }
        break;
    case Node::String:
        if(typeToCastTo == Node::String)
        {
            implicitCastRightChild = Node::String;
            returnType = Node::String;
        }
        else
        {
            implicitCastRightChild = Node::ErrorType;
            returnType = Node::ErrorType;
        }
        break;
    case Node::ErrorType:
        implicitCastRightChild = Node::ErrorType;
        returnType = Node::ErrorType;
        break;
    }
}

OperationNode::Operation TypeCastNode::getOp() const
{
    return OperationNode::TypeCast;
}

OperationNode::Associativity TypeCastNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence TypeCastNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> TypeCastNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    switch(typeToCastTo)
    {
    case Node::Integer:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>()));
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>()));
        break;
    case Node::Bool:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<bool>()));
        break;
    case Node::String:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<QString>()));
        break;
    case Node::ErrorType:
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString TypeCastNode::printValue() const
{
    QString TypeClearName;
    switch(typeToCastTo)
    {
    case Node::Integer:
        TypeClearName = QString("Integer");
        break;
    case Node::Double:
        TypeClearName = QString("Double");
        break;
    case Node::Bool:
        TypeClearName = QString("Bool");
        break;
    case Node::String:
        TypeClearName = QString("String");
        break;
    case Node::ErrorType:
        TypeClearName = QString("ErrorType");
        break;
    }

    return QString("(%1)").arg(TypeClearName);
}

QString TypeCastNode::printNode() const
{
    return QString("{(TypeCast):(%1)}").arg(printValue());
}

IncrementNode::IncrementNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if(returnType == Node::Integer)
    {
        implicitCastRightChild = Node::Integer;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation IncrementNode::getOp() const
{
    return OperationNode::Increment;
}

OperationNode::Associativity IncrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence IncrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> IncrementNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    ValueNode Result = ValueNode(value->getValue().value<int>() + 1);

    dynamic_cast<VariableNode*>(UnaryOPRightChild.get())->getRelatedVariableSymbol()->assignValue(Result);

    return std::unique_ptr<ValueNode>( new ValueNode(Result));
}

QString IncrementNode::printValue() const
{
    return QString("++");
}

QString IncrementNode::printNode() const
{
    QString NodeType = "IncrementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

DecrementNode::DecrementNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if(returnType == Node::Integer)
    {
        implicitCastRightChild = Node::Integer;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation DecrementNode::getOp() const
{
    return OperationNode::Decrement;
}

OperationNode::Associativity DecrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence DecrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> DecrementNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    ValueNode Result = ValueNode(value->getValue().value<int>() - 1);

    dynamic_cast<VariableNode*>(UnaryOPRightChild.get())->getRelatedVariableSymbol()->assignValue(Result);

    return std::unique_ptr<ValueNode>( new ValueNode(Result));
}

QString DecrementNode::printValue() const
{
    return QString("--");
}

QString DecrementNode::printNode() const
{
    QString NodeType = "DecrementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

PositiveNode::PositiveNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( returnType == Node::Integer )
    {
        implicitCastRightChild = Node::Integer;
    }
    else if( returnType == Node::Double )
    {
        implicitCastRightChild = Node::Double;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation PositiveNode::getOp() const
{
    return OperationNode::Positive;
}

OperationNode::Associativity PositiveNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence PositiveNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> PositiveNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    switch(implicitCastRightChild)
    {
    case Node::Integer:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>()));
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>()));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString PositiveNode::printValue() const
{
    return QString("+");
}

QString PositiveNode::printNode() const
{
    QString NodeType = "PositiveNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

NegativeNode::NegativeNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( returnType == Node::Integer )
    {
        implicitCastRightChild = Node::Integer;
    }
    else if( returnType == Node::Double )
    {
        implicitCastRightChild = Node::Double;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation NegativeNode::getOp() const
{
    return OperationNode::Negative;
}

OperationNode::Associativity NegativeNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence NegativeNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> NegativeNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    switch(implicitCastRightChild)
    {
    case Node::Integer:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<int>() * -1));
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value->getValue().value<double>() * -1));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString NegativeNode::printValue() const
{
    return QString("-");
}

QString NegativeNode::printNode() const
{
    QString NodeType = "NegativeNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnaryLogicalOperationNode::UnaryLogicalOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalNegationNode::LogicalNegationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryLogicalOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( ( returnType != Node::Integer ) && ( returnType != Node::Double )  && ( returnType != Node::Bool ) )
        returnType = Node::ErrorType;
}

OperationNode::Operation LogicalNegationNode::getOp() const
{
    return OperationNode::LogicalNegation;
}

OperationNode::Associativity LogicalNegationNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence LogicalNegationNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> LogicalNegationNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    bool IsTrue = value->getValue().value<bool>();

    switch(implicitCastRightChild)
    {
    case Node::Integer:
        return std::unique_ptr<ValueNode>( new ValueNode(IsTrue ? 0 : 1));
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(IsTrue ? 0.0 : 1.0));
        break;
    case Node::Bool:
        return std::unique_ptr<ValueNode>( new ValueNode(IsTrue ? false : true));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString LogicalNegationNode::printValue() const
{
    return QString("!");
}

QString LogicalNegationNode::printNode() const
{
    QString NodeType = "LogicalNegationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnaryBitwiseOperationNode::UnaryBitwiseOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

OnesComplementNode::OnesComplementNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryBitwiseOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( ( returnType != Node::Integer ) )
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation OnesComplementNode::getOp() const
{
    return OperationNode::OnesComplement;
}

OperationNode::Associativity OnesComplementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence OnesComplementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

std::unique_ptr<ValueNode> OnesComplementNode::DoOperation() const
{
    std::unique_ptr<ValueNode> value = UnaryOPRightChild->visit();

    return std::unique_ptr<ValueNode>( new ValueNode(~(value->getValue().value<int>())));
}

QString OnesComplementNode::printValue() const
{
    return QString("~");
}

QString OnesComplementNode::printNode() const
{
    QString NodeType = "OnesComplementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}
