#include "unaryoperationnodes.h"

UnaryArithmeticOperationNode::UnaryArithmeticOperationNode(const SimpleNode &rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TypeCastNode::TypeCastNode(const SimpleNode &rightChild, SimpleNode::ValueTypes typeToCastTo) :
    UnaryArithmeticOperationNode(rightChild),
    typeToCastTo(typeToCastTo)
{
    returnType = rightChild->getReturnType();

    switch(returnType)
    {
    case SimpleNode::Integer:
    case SimpleNode::Double:
    case SimpleNode::Bool:
        if(typeToCastTo != SimpleNode::ErrorType )
        {
            implicitCastRightChild = typeToCastTo;
            returnType = typeToCastTo;
        }
        else
        {
            implicitCastRightChild = SimpleNode::ErrorType;
            returnType = ErrorType;
        }
        break;
    case SimpleNode::String:
        if(typeToCastTo == SimpleNode::String)
        {
            implicitCastRightChild = SimpleNode::String;
            returnType = SimpleNode::String;
        }
        else
        {
            implicitCastRightChild = SimpleNode::ErrorType;
            returnType = ErrorType;
        }
        break;
    case SimpleNode::ErrorType:
        implicitCastRightChild = SimpleNode::ErrorType;
        returnType = ErrorType;
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

ValueNode &TypeCastNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    switch(typeToCastTo)
    {
    case SimpleNode::Integer:
        Result = ValueNode(value.getValue().value<int>());
        break;
    case SimpleNode::Double:
        Result = ValueNode(value.getValue().value<double>());
        break;
    case SimpleNode::Bool:
        Result = ValueNode(value.getValue().value<bool>());
        break;
    case SimpleNode::String:
        Result = ValueNode(value.getValue().value<QString>());
        break;
    case SimpleNode::ErrorType:
        Result = ValueNode();
        break;
    }

    return Result;
}

QString TypeCastNode::printValue() const
{
    QString TypeClearName;
    switch(typeToCastTo)
    {
    case SimpleNode::Integer:
        TypeClearName = QString("Integer");
        break;
    case SimpleNode::Double:
        TypeClearName = QString("Double");
        break;
    case SimpleNode::Bool:
        TypeClearName = QString("Bool");
        break;
    case SimpleNode::String:
        TypeClearName = QString("String");
        break;
    case SimpleNode::ErrorType:
        TypeClearName = QString("ErrorType");
        break;
    }

    return QString("(%1)").arg(TypeClearName);
}

QString TypeCastNode::printNode() const
{
    return QString("{(TypeCast):(%1)}").arg(printValue());
}

IncrementNode::IncrementNode(const SimpleNode &rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if(returnType == ValueNode::Integer)
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else
    {
        returnType = ValueNode::ErrorType;
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

ValueNode &IncrementNode::DoOperation()
{
    bool IsVariable = false;
    ValueNode &value = rightChild->visit();

    Result = ValueNode(value.getValue().value<int>() + 1);

    dynamic_cast<VariableNode*>(rightChild)->setAssignment(new ValueNode(Result));

    return Result;
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

DecrementNode::DecrementNode(const SimpleNode &rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if(returnType == ValueNode::Integer)
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else
    {
        returnType = ValueNode::ErrorType;
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

ValueNode &DecrementNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    Result = ValueNode(value.getValue().value<int>() - 1);

    dynamic_cast<VariableNode*>(rightChild)->setAssignment(new ValueNode(Result));

    return Result;
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

PositiveNode::PositiveNode(const SimpleNode &rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( returnType == ValueNode::Integer )
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else if( returnType == ValueNode::Double )
    {
        implicitCastRightChild = SimpleNode::Double;
    }
    else
    {
        returnType = ValueNode::ErrorType;
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

ValueNode &PositiveNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result = ValueNode(value.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
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

NegativeNode::NegativeNode(const SimpleNode &rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( returnType == ValueNode::Integer )
    {
        implicitCastRightChild = SimpleNode::Integer;
    }
    else if( returnType == ValueNode::Double )
    {
        implicitCastRightChild = SimpleNode::Double;
    }
    else
    {
        returnType = ValueNode::ErrorType;
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

ValueNode &NegativeNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>() * -1);
        break;
    case ValueNode::Double:
        Result = ValueNode(value.getValue().value<double>() * -1);
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
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

UnaryLogicalOperationNode::UnaryLogicalOperationNode(const SimpleNode &rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalNegationNode::LogicalNegationNode(const SimpleNode &rightChild) :
    UnaryLogicalOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( ( returnType != ValueNode::Integer ) && ( returnType != ValueNode::Double )  && ( returnType != ValueNode::Bool ) )
        returnType = ValueNode::ErrorType;
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

ValueNode &LogicalNegationNode::DoOperation()
{
    ValueNode &value = rightChild->visit();
    bool IsTrue = value.getValue().value<bool>();
    switch(implicitCastRightChild)
    {
    case ValueNode::Integer:
        Result = ValueNode(IsTrue ? 0 : 1);
        break;
    case ValueNode::Double:
        Result = ValueNode(IsTrue ? 0.0 : 1.0);
        break;
    case ValueNode::Bool:
        Result =  ValueNode(IsTrue ? false : true);
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
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

UnaryBitwiseOperationNode::UnaryBitwiseOperationNode(const SimpleNode &rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

OnesComplementNode::OnesComplementNode(const SimpleNode &rightChild) :
    UnaryBitwiseOperationNode(rightChild)
{
    returnType = rightChild->getReturnType();
    if( ( returnType != ValueNode::Integer ) )
        returnType = ValueNode::ErrorType;
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

ValueNode &OnesComplementNode::DoOperation()
{
    ValueNode &value = rightChild->visit();

    Result =  ValueNode(~(value.getValue().value<int>()));

    return Result;
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
