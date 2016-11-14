#include "simpleast.h"

#include <QDebug>

SimpleNode::SimpleNode()
{
}

SimpleNode::NodeType SimpleNode::getNodeType() const
{
    SimpleNode::EOFNode;
}

ValueNode::ValueNode() :
    SimpleNode(SimpleNode::Value),
    valueType(ErrorType),
    value(QVariant())
{
}

ValueNode::ValueNode(NodeToCopy) :
    SimpleNode(SimpleNode::Value),
    valueType(valueNodeToCopy.getValueType()),
    value(valueNodeToCopy.getValue())
{
}

ValueNode::ValueNode(ValueNode &&valueNodeToMove) :
    SimpleNode(SimpleNode::Value),
    valueType(valueNodeToMove.getValueType()),
    value(valueNodeToMove.getValue())
{
}

ValueNode::ValueNode(const int value) :
    SimpleNode(SimpleNode::Value),
    valueType(Integer),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const double value) :
    SimpleNode(SimpleNode::Value),
    valueType(Double),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const bool value) :
    SimpleNode(SimpleNode::Value),
    valueType(Bool),
    value(QVariant::fromValue(value))
{
}

ValueNode::ValueNode(const QString &value) :
    SimpleNode(SimpleNode::Value),
    valueType(String),
    value(QVariant::fromValue(value))
{
}

SimpleNode::NodeType ValueNode::getNodeType() const
{
    return SimpleNode::Value;
}

ValueNode &ValueNode::operator=(const ValueNode &value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
}

const QVariant ValueNode::getValue() const
{
    return value;
}

ValueNode::ValueTypes ValueNode::getValueType() const
{
    return valueType;
}

QString ValueNode::printValue() const
{
    switch(valueType)
    {
    case Integer:
        return QString::number(value.value<int>());
        break;
    case Double:
        return QString::number(value.value<double>());
        break;
    case Bool:
        return QString(value.value<bool>() ? "true" : "false");
        break;
    case String:
        return value.value<QString>();
        break;
    case ErrorType:
    default:
        return QString("ERROR");
    }
}

QString ValueNode::printNode() const
{
    QString NodeType = "ValueNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

ValueNode &ValueNode::visit()
{
    return *this;
}

DataNode::DataNode(const unsigned int dataIndex, const SymbolTable * const SymblTbl) :
    SimpleNode(SimpleNode::Data),
    dataIndex(dataIndex),
    SymblTable(SymblTbl)
{
}

SimpleNode::NodeType DataNode::getNodeType() const
{
    return SimpleNode::Data;
}

ValueNode &DataNode::visit()
{
    Result = ValueNode(SymblTbl->lookup(QString("D%1").arg(dataIndex)));
    return Result;
}

QString DataNode::printValue() const
{
    return QString("D%1").arg(dataIndex);
}

QString DataNode::printNode() const
{
    QString NodeType = "DataNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

OperationNode::OperationNode() :
    SimpleNode(SimpleNode::Operation)
{
}

SimpleNode::NodeType OperationNode::getNodeType() const
{
    return SimpleNode::Operation;
}

ValueNode &OperationNode::visit()
{
    return DoOperation();
}

UnaryOperationNode::UnaryOperationNode(SimpleNode *rightChild) :
    rightChild(rightChild)
{
}

OperationNode::ArityTypes UnaryOperationNode::getArityType() const
{
    return OperationNode::Unary;
}

UnaryArithmeticOperationNode::UnaryArithmeticOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{

}

OperationNode::OperationTypes UnaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

IncrementNode::IncrementNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{

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

const ValueNode &IncrementNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>() + 1);
        break;
    default:
        Result =  ValueNode();
    }
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

DecrementNode::DecrementNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
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

const ValueNode &DecrementNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
    {
    case ValueNode::Integer:
        Result = ValueNode(value.getValue().value<int>() - 1);
        break;
    default:
        Result =  ValueNode();
    }
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

PositiveNode::PositiveNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
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

const ValueNode &PositiveNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
    {
    case ValueNode::Integer:
    case ValueNode::Double:
        Result = ValueNode(value.getValue());
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

NegativeNode::NegativeNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
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

const ValueNode &NegativeNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
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

UnaryLogicalOperationNode::UnaryLogicalOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalNegationNode::LogicalNegationNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
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

const ValueNode &LogicalNegationNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
    {
    case ValueNode::Integer:
    case ValueNode::Double:
    case ValueNode::Bool:
        if(value.getValue().value<bool>())
            Result =  ValueNode(false);
        else
            Result =  ValueNode(true);
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

UnaryBitwiseOperationNode::UnaryBitwiseOperationNode(SimpleNode *rightChild) :
    UnaryOperationNode(rightChild)
{
}

OperationNode::OperationTypes UnaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

OnesComplementNode::OnesComplementNode(SimpleNode *rightChild) :
    UnaryArithmeticOperationNode(rightChild)
{
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

const ValueNode &OnesComplementNode::DoOperation() const
{
    ValueNode &value = rightChild->visit();
    switch(value.getValueType())
    {
    case ValueNode::Integer:
        Result =  ValueNode(~(value.getValue().value<int>()));
        break;
    default:
        Result =  ValueNode();
    }
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

BinaryOperationNode::BinaryOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    leftChild(leftChild),
    rightChild(rightChild)
{
}

OperationNode::ArityTypes BinaryOperationNode::getArityType() const
{
    return OperationNode::Binary;
}

BinaryArithmeticOperationNode::BinaryArithmeticOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{

}

OperationNode::OperationTypes BinaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

AdditionNode::AdditionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation AdditionNode::getOp() const
{
    return OperationNode::Addition;
}

OperationNode::Associativity AdditionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence AdditionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

const ValueNode &AdditionNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();

    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result =  ValueNode(value1.getValue().value<int>() + value2.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() + value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString AdditionNode::printValue() const
{
    return QString("+");
}

QString AdditionNode::printNode() const
{
    QString NodeType = "AdditionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

SubtractionNode::SubtractionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation SubtractionNode::getOp() const
{
    return OperationNode::Subtraction;
}

OperationNode::Associativity SubtractionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence SubtractionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

const ValueNode &SubtractionNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result =  ValueNode(value1.getValue().value<int>() - value2.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() - value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString SubtractionNode::printValue() const
{
    return QString("-");
}

QString SubtractionNode::printNode() const
{
    QString NodeType = "SubtractionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

MultiplicationNode::MultiplicationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation MultiplicationNode::getOp() const
{
    return OperationNode::Multiplication;
}

OperationNode::Associativity MultiplicationNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence MultiplicationNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

const ValueNode &MultiplicationNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result =  ValueNode(value1.getValue().value<int>() * value2.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() * value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString MultiplicationNode::printValue() const
{
    return QString("*");
}

QString MultiplicationNode::printNode() const
{
    QString NodeType = "MultiplicationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

DivisionNode::DivisionNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation DivisionNode::getOp() const
{
    return OperationNode::Division;
}

OperationNode::Associativity DivisionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence DivisionNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

const ValueNode &DivisionNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result =  ValueNode(value1.getValue().value<int>() / value2.getValue().value<int>());
        break;
    case ValueNode::Double:
        Result =  ValueNode(value1.getValue().value<double>() / value2.getValue().value<double>());
        break;
    default:
        Result =  ValueNode();
    }
    return Result;
}

QString DivisionNode::printValue() const
{
    return QString("/");
}

QString DivisionNode::printNode() const
{
    QString NodeType = "DivisionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

ModuloNode::ModuloNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation ModuloNode::getOp() const
{
    return OperationNode::Modulo;
}

OperationNode::Associativity ModuloNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ModuloNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

const ValueNode &ModuloNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result =  ValueNode(value1.getValue().value<int>() % value2.getValue().value<int>());
    return Result;
}

QString ModuloNode::printValue() const
{
    return QString("%");
}

QString ModuloNode::printNode() const
{
    QString NodeType = "ModuloNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

BinaryLogicalOperationNode::BinaryLogicalOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{

}

OperationNode::OperationTypes BinaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalANDNode::LogicalANDNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation LogicalANDNode::getOp() const
{
    return OperationNode::LogicalAND;
}

OperationNode::Associativity LogicalANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalANDNode::getPrecedence() const
{
    return OperationNode::LogicalANDPrec;
}

const ValueNode &LogicalANDNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result =  ValueNode(value1.getValue().value<bool>() && value2.getValue().value<bool>());
    return Result;
}

QString LogicalANDNode::printValue() const
{
    return QString("&&");
}

QString LogicalANDNode::printNode() const
{
    QString NodeType = "LogicalANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LogicalORNode::LogicalORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation LogicalORNode::getOp() const
{
    return OperationNode::LogicalOR;
}

OperationNode::Associativity LogicalORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

const ValueNode &LogicalORNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result =  ValueNode(value1.getValue().value<bool>() || value2.getValue().value<bool>());
    return Result;
}

QString LogicalORNode::printValue() const
{
    return QString("||");
}

QString LogicalORNode::printNode() const
{
    QString NodeType = "LogicalORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LogicalXORNode::LogicalXORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation LogicalXORNode::getOp() const
{
    return OperationNode::LogicalXOR;
}

OperationNode::Associativity LogicalXORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalXORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

const ValueNode &LogicalXORNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result = ValueNode(value1.getValue().value<bool>() != value2.getValue().value<bool>());
    return Result;
}

QString LogicalXORNode::printValue() const
{
    return QString("^^");
}

QString LogicalXORNode::printNode() const
{
    QString NodeType = "LogicalXORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

GreaterNode::GreaterNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation GreaterNode::getOp() const
{
    return OperationNode::Greater;
}

OperationNode::Associativity GreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence GreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

const ValueNode &GreaterNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result = (value1.getValue().value<double>() > value2.getValue().value<double>()) ?  ValueNode(true) : ValueNode(false);
    return Result;
}

QString GreaterNode::printValue() const
{
    return QString(">");
}

QString GreaterNode::printNode() const
{
    QString NodeType = "GreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LowerNode::LowerNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation LowerNode::getOp() const
{
    return OperationNode::Lower;
}

OperationNode::Associativity LowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

const ValueNode &LowerNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    Result = (value1.getValue().value<double>() < value2.getValue().value<double>()) ?  ValueNode(true) : ValueNode(false);
    return Result;
}

QString LowerNode::printValue() const
{
    return QString("<");
}

QString LowerNode::printNode() const
{
    QString NodeType = "LowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualNode::EqualNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation EqualNode::getOp() const
{
    return OperationNode::Equal;
}

OperationNode::Associativity EqualNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

const ValueNode &EqualNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result = (value1.getValue().value<int>() == value2.getValue().value<int>()) ? ValueNode(true) : ValueNode(false);
        break;
    case ValueNode::Double:
        Result = (value1.getValue().value<double>() == value2.getValue().value<double>()) ? ValueNode(true) : ValueNode(false);
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualNode::printValue() const
{
    return QString("==");
}

QString EqualNode::printNode() const
{
    QString NodeType = "EqualNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualOrGreaterNode::EqualOrGreaterNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation EqualOrGreaterNode::getOp() const
{
    return OperationNode::EqualOrGreater;
}

OperationNode::Associativity EqualOrGreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrGreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

const ValueNode &EqualOrGreaterNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result = (value1.getValue().value<int>() >= value2.getValue().value<int>()) ? ValueNode(true) : ValueNode(false);
        break;
    case ValueNode::Double:
        Result = (value1.getValue().value<double>() >= value2.getValue().value<double>()) ? ValueNode(true) : ValueNode(false);
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualOrGreaterNode::printValue() const
{
    return QString(">=");
}

QString EqualOrGreaterNode::printNode() const
{
    QString NodeType = "EqualOrGreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

EqualOrLowerNode::EqualOrLowerNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation EqualOrLowerNode::getOp() const
{
    return OperationNode::EqualOrLower;
}

OperationNode::Associativity EqualOrLowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrLowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

const ValueNode &EqualOrLowerNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result = (value1.getValue().value<int>() <= value2.getValue().value<int>()) ? ValueNode(true) : ValueNode(false);
        break;
    case ValueNode::Double:
        Result = (value1.getValue().value<double>() <= value2.getValue().value<double>()) ? ValueNode(true) : ValueNode(false);
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString EqualOrLowerNode::printValue() const
{
    return QString("<=");
}

QString EqualOrLowerNode::printNode() const
{
    QString NodeType = "EqualOrLowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

UnequalNode::UnequalNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation UnequalNode::getOp() const
{
    return OperationNode::Unequal;
}

OperationNode::Associativity UnequalNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence UnequalNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

const ValueNode &UnequalNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    ValueNode::ValueTypes resultType = ValueNode::Integer;
    if( ( value1.getValueType() == ValueNode::Double ) || ( value2.getValueType() == ValueNode::Double ) )
        resultType = ValueNode::Double;

    switch(resultType)
    {
    case ValueNode::Integer:
        Result = (value1.getValue().value<int>() != value2.getValue().value<int>()) ? ValueNode(true) : ValueNode(false);
        break;
    case ValueNode::Double:
        Result = (value1.getValue().value<double>() != value2.getValue().value<double>()) ? ValueNode(true) : ValueNode(false);
        break;
    default:
        Result = ValueNode();
    }
    return Result;
}

QString UnequalNode::printValue() const
{
    return QString("!=");
}

QString UnequalNode::printNode() const
{
    QString NodeType = "UneqalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

BinaryBitwiseOperationNode::BinaryBitwiseOperationNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

ANDNode::ANDNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation ANDNode::getOp() const
{
    return OperationNode::AND;
}

OperationNode::Associativity ANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ANDNode::getPrecedence() const
{
    return OperationNode::BitwiseANDPrec;
}

const ValueNode &ANDNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    //ToThink is this really needed??
    if((value1.getValueType() != ValueNode::Integer) || (value2.getValueType() != ValueNode::Integer))
        Result = ValueNode();
    else
        Result = ValueNode(value1.getValue().value<int>() & value2.getValue().value<int>());

    return Result;
}

QString ANDNode::printValue() const
{
    return QString("&");
}

QString ANDNode::printNode() const
{
    QString NodeType = "ANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

ORNode::ORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation ORNode::getOp() const
{
    return OperationNode::OR;
}

OperationNode::Associativity ORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ORNode::getPrecedence() const
{
    return OperationNode::BitwiseORPrec;
}

const ValueNode &ORNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    //ToThink is this really needed??
    if((value1.getValueType() != ValueNode::Integer) || (value2.getValueType() != ValueNode::Integer))
        Result = ValueNode();
    else
        Result = ValueNode(value1.getValue().value<int>() | value2.getValue().value<int>());

    return Result;
}

QString ORNode::printValue() const
{
    return QString("|");
}

QString ORNode::printNode() const
{
    QString NodeType = "ORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

XORNode::XORNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation XORNode::getOp() const
{
    return OperationNode::XOR;
}

OperationNode::Associativity XORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence XORNode::getPrecedence() const
{
    return OperationNode::BitwiseXORPrec;
}

const ValueNode &XORNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    //ToThink is this really needed??
    if((value1.getValueType() != ValueNode::Integer) || (value2.getValueType() != ValueNode::Integer))
        Result = ValueNode();
    else
        Result = ValueNode(value1.getValue().value<int>() ^ value2.getValue().value<int>());

    return Result;
}

QString XORNode::printValue() const
{
    return QString("^");
}

QString XORNode::printNode() const
{
    QString NodeType = "XORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

LeftShiftNode::LeftShiftNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation LeftShiftNode::getOp() const
{
    return OperationNode::LeftShift;
}

OperationNode::Associativity LeftShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LeftShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

const ValueNode &LeftShiftNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    //ToThink is this really needed??
    if((value1.getValueType() != ValueNode::Integer) || (value2.getValueType() != ValueNode::Integer))
        Result = ValueNode();
    else
        Result = ValueNode(value1.getValue().value<int>() << value2.getValue().value<int>());

    return Result;
}

QString LeftShiftNode::printValue() const
{
    return QString("<<");
}

QString LeftShiftNode::printNode() const
{
    QString NodeType = "LeftShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

RightShiftNode::RightShiftNode(SimpleNode *leftChild, SimpleNode *rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
}

OperationNode::Operation RightShiftNode::getOp() const
{
    return OperationNode::RightShift;
}

OperationNode::Associativity RightShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence RightShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

const ValueNode &RightShiftNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode &value2 = leftChild->visit();
    //ToThink is this really needed??
    if((value1.getValueType() != ValueNode::Integer) || (value2.getValueType() != ValueNode::Integer))
        Result = ValueNode();
    else
        Result = ValueNode(value1.getValue().value<int>() >> value2.getValue().value<int>());

    return Result;
}

QString RightShiftNode::printValue() const
{
    return QString(">>");
}

QString RightShiftNode::printNode() const
{
    QString NodeType = "RightShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

TernaryOperationNode::TernaryOperationNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    leftChild(leftChild),
    midChild(midChild),
    rightChild(rightChild)
{
}

OperationNode::ArityTypes TernaryOperationNode::getArityType() const
{
    return OperationNode::Ternary;
}

ConditionalNode::ConditionalNode(SimpleNode *leftChild, SimpleNode *midChild, SimpleNode *rightChild) :
    TernaryOperationNode(leftChild, midChild, rightChild)
{
}

OperationNode::OperationTypes ConditionalNode::getOpType() const
{
    return OperationNode::Logical;
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

const ValueNode &ConditionalNode::DoOperation() const
{
    ValueNode &value1 = rightChild->visit();
    ValueNode value2 = midChild->visit();
    ValueNode &value3 = leftChild->visit();

    Result = (value1.getValue().value<bool>()) ? ValueNode(value2) : ValueNode(value3);

    return Result;
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


EOFNode::EOFNode()
{
}

SimpleNode::NodeType EOFNode::getNodeType() const
{
    return SimpleNode::EOFNode;
}

QString EOFNode::printValue() const
{
    return QString("EOF");
}

QString EOFNode::printNode() const
{
    return QString("{(EOF):(EOF)}");
}

ValueNode &EOFNode::visit()
{
    return InvalidValue;
}
