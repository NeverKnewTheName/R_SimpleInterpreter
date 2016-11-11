#include "simpletoken.h"

#include <QDebug>

SimpleToken::SimpleToken(const SimpleToken::TokenType type) :
    type(type)
{
}

SimpleToken::TokenType SimpleToken::getTokenType() const
{
    return type;
}

ValueToken::ValueToken() :
    SimpleToken(SimpleToken::Value),
    valueType(ErrorType),
    value(QVariant())
{
}

ValueToken::ValueToken(const ValueToken &valueTokenToCopy) :
    SimpleToken(SimpleToken::Value),
    valueType(valueTokenToCopy.getValueType()),
    value(valueTokenToCopy.getValue())
{
}

ValueToken::ValueToken(const int value) :
    SimpleToken(SimpleToken::Value),
    valueType(Integer),
    value(QVariant::fromValue(value))
{
}

ValueToken::ValueToken(const double value) :
    SimpleToken(SimpleToken::Value),
    valueType(Double),
    value(QVariant::fromValue(value))
{
}

ValueToken::ValueToken(const bool value) :
    SimpleToken(SimpleToken::Value),
    valueType(Bool),
    value(QVariant::fromValue(value))
{
}

ValueToken::ValueToken(const QString &value) :
    SimpleToken(SimpleToken::Value),
    valueType(String),
    value(QVariant::fromValue(value))
{
}

const QVariant ValueToken::getValue() const
{
    return value;
}

ValueToken::ValueTypes ValueToken::getValueType() const
{
    return valueType;
}

QString ValueToken::printValue() const
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

QString ValueToken::printToken() const
{
    QString tokenType = "ValueToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

DataToken::DataToken(const unsigned int dataIndex) :
    SimpleToken(SimpleToken::Data),
    dataIndex(dataIndex)
{
}

const ValueToken DataToken::getData(const QVector<quint8> &dataVector) const
{
    return ValueToken(dataVector.at(dataIndex));
}

QString DataToken::printValue() const
{
    return QString("D%1").arg(dataIndex);
}

QString DataToken::printToken() const
{
    QString tokenType = "DataToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

OperationToken::OperationToken(const OperationToken::ArityTypes arityType) :
    SimpleToken(SimpleToken::Operation),
    arityType(arityType)
{
}

OperationToken::ArityTypes OperationToken::getArityType() const
{
    return arityType;
}



UnaryOperationToken::UnaryOperationToken(const UnaryOperationToken::OperationTypes opType) :
    OperationToken(OperationToken::Unary),
    opType(opType)
{

}

UnaryOperationToken::OperationTypes UnaryOperationToken::getOpType() const
{
    return opType;
}

UnaryArithmeticOperationToken::UnaryArithmeticOperationToken(const UnaryArithmeticOperationToken::Operation op) :
    UnaryOperationToken(UnaryOperationToken::Arithmetic),
    op(op)
{
}

UnaryArithmeticOperationToken::Operation UnaryArithmeticOperationToken::getOp() const
{
    return op;
}

IncrementToken::IncrementToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Increment)
{
}

const ValueToken IncrementToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>() + 1);
        break;
    default:
        return ValueToken();
    }
}

DecrementToken::DecrementToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Decrement)
{
}

const ValueToken DecrementToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>() - 1);
        break;
    default:
        return ValueToken();
    }
}

PositiveToken::PositiveToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Positive)
{
}

const ValueToken PositiveToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(value.getValue().value<double>());
        break;
    default:
        return ValueToken();
    }
}

NegativeToken::NegativeToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Negative)
{
}

const ValueToken NegativeToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>() * -1);
        break;
    case ValueToken::Double:
        return ValueToken(value.getValue().value<double>() * -1);
        break;
    default:
        return ValueToken();
    }
}

UnaryLogicalOperationToken::UnaryLogicalOperationToken(const UnaryLogicalOperationToken::Operation op) :
    UnaryOperationToken(UnaryOperationToken::Logical),
    op(op)
{
}

UnaryLogicalOperationToken::Operation UnaryLogicalOperationToken::getOp() const
{
    return op;
}

LogicalNegationToken::LogicalNegationToken() :
    UnaryLogicalOperationToken(UnaryLogicalOperationToken::LogicalNegation)
{
}

const ValueToken LogicalNegationToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
    case ValueToken::Double:
    case ValueToken::Bool:
        if(value.getValue().value<bool>())
            return ValueToken(false);
        else
            return ValueToken(true);
        break;
    default:
        return ValueToken();
    }
}

UnaryBitwiseOperationToken::UnaryBitwiseOperationToken(const UnaryBitwiseOperationToken::Operation op) :
    UnaryOperationToken(UnaryOperationToken::Bitwise),
    op(op)
{
}

UnaryBitwiseOperationToken::Operation UnaryBitwiseOperationToken::getOp() const
{
    return op;
}

OnesComplementToken::OnesComplementToken() :
    UnaryBitwiseOperationToken(UnaryBitwiseOperationToken::OnesComplement)
{
}

const ValueToken OnesComplementToken::DoOperation(const ValueToken &value) const
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(~(value.getValue().value<int>()));
            break;
    default:
        return ValueToken();
    }
}

BinaryOperationToken::BinaryOperationToken(const BinaryOperationToken::OperationTypes opType) :
    OperationToken(OperationToken::Binary),
    opType(opType)
{
}

BinaryOperationToken::OperationTypes BinaryOperationToken::getOpType() const
{
    return opType;
}

BinaryArithmeticOperationToken::BinaryArithmeticOperationToken(const BinaryArithmeticOperationToken::Operation op) :
    BinaryOperationToken(BinaryOperationToken::Arithmetic),
    op(op)
{
}

BinaryArithmeticOperationToken::Operation BinaryArithmeticOperationToken::getOp() const
{
    return op;
}

AdditionToken::AdditionToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Addition)
{
}

const ValueToken AdditionToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return ValueToken(value1.getValue().value<int>() + value2.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(value1.getValue().value<double>() + value2.getValue().value<double>());
        break;
    default:
        return ValueToken();
    }
}

SubtractionToken::SubtractionToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Subtraction)
{
}

const ValueToken SubtractionToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return ValueToken(value1.getValue().value<int>() - value2.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(value1.getValue().value<double>() - value2.getValue().value<double>());
        break;
    default:
        return ValueToken();
    }
}

MultiplicationToken::MultiplicationToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Multiplication)
{
}

const ValueToken MultiplicationToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return ValueToken(value1.getValue().value<int>() * value2.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(value1.getValue().value<double>() * value2.getValue().value<double>());
        break;
    default:
        return ValueToken();
    }
}

DivisionToken::DivisionToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Division)
{
}

const ValueToken DivisionToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return ValueToken(value1.getValue().value<int>() / value2.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(value1.getValue().value<double>() / value2.getValue().value<double>());
        break;
    default:
        return ValueToken();
    }
}

ModuloToken::ModuloToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Modulo)
{
}

const ValueToken ModuloToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<int>() % value2.getValue().value<int>());
}

BinaryLogicalOperationToken::BinaryLogicalOperationToken(const BinaryLogicalOperationToken::Operation op) :
    BinaryOperationToken(BinaryOperationToken::Logical),
    op(op)
{
}

BinaryLogicalOperationToken::Operation BinaryLogicalOperationToken::getOpType() const
{
    return op;
}

LogicalANDToken::LogicalANDToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::AND)
{
}

const ValueToken LogicalANDToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() && value2.getValue().value<bool>());
}

LogicalORToken::LogicalORToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::OR)
{
}

const ValueToken LogicalORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() && value2.getValue().value<bool>());
}

LogicalXORToken::LogicalXORToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::XOR)
{
}

const ValueToken LogicalXORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() != value2.getValue().value<bool>());
}

GreaterToken::GreaterToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::Greater)
{
}

const ValueToken GreaterToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return (value1.getValue().value<double>() > value2.getValue().value<double>()) ?  ValueToken(true) : ValueToken(false);
}

LowerToken::LowerToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::Lower)
{
}

const ValueToken LowerToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return (value1.getValue().value<double>() < value2.getValue().value<double>()) ?  ValueToken(true) : ValueToken(false);
}

EqualToken::EqualToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::Equal)
{
}

const ValueToken EqualToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return (value1.getValue().value<int>() == value2.getValue().value<int>()) ? ValueToken(true) : ValueToken(false);
        break;
    case ValueToken::Double:
        return (value1.getValue().value<double>() == value2.getValue().value<double>()) ? ValueToken(true) : ValueToken(false);
        break;
    default:
        return ValueToken();
    }
}

EqualOrGreaterToken::EqualOrGreaterToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::EqualOrGreater)
{
}

const ValueToken EqualOrGreaterToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return (value1.getValue().value<int>() >= value2.getValue().value<int>()) ? ValueToken(true) : ValueToken(false);
        break;
    case ValueToken::Double:
        return (value1.getValue().value<double>() >= value2.getValue().value<double>()) ? ValueToken(true) : ValueToken(false);
        break;
    default:
        return ValueToken();
    }
}

EqualOrLowerToken::EqualOrLowerToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::EqualOrLower)
{
}

const ValueToken EqualOrLowerToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return (value1.getValue().value<int>() <= value2.getValue().value<int>()) ? ValueToken(true) : ValueToken(false);
        break;
    case ValueToken::Double:
        return (value1.getValue().value<double>() <= value2.getValue().value<double>()) ? ValueToken(true) : ValueToken(false);
        break;
    default:
        return ValueToken();
    }
}

UneqalToken::UneqalToken() :
    BinaryLogicalOperationToken(BinaryLogicalOperationToken::Unequal)
{
}

const ValueToken UneqalToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    ValueToken::ValueTypes resultType = ValueToken::Integer;
    if( ( value1.getValueType() == ValueToken::Double ) || ( value2.getValueType() == ValueToken::Double ) )
        resultType = ValueToken::Double;

    switch(resultType)
    {
    case ValueToken::Integer:
        return (value1.getValue().value<int>() != value2.getValue().value<int>()) ? ValueToken(true) : ValueToken(false);
        break;
    case ValueToken::Double:
        return (value1.getValue().value<double>() != value2.getValue().value<double>()) ? ValueToken(true) : ValueToken(false);
        break;
    default:
        return ValueToken();
    }
}

BinaryBitwiseOperationToken::BinaryBitwiseOperationToken(const BinaryBitwiseOperationToken::Operation op) :
    BinaryOperationToken(BinaryOperationToken::Bitwise),
    op(op)
{
}

BinaryBitwiseOperationToken::Operation BinaryBitwiseOperationToken::getOp() const
{
    return op;
}

ANDToken::ANDToken() :
    BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::AND)
{
}

const ValueToken ANDToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() & value2.getValue().value<int>());
}

ORToken::ORToken() :
    BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::OR)
{
}

const ValueToken ORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() | value2.getValue().value<int>());
}

XORToken::XORToken() :
    BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::XOR)
{
}

const ValueToken XORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() ^ value2.getValue().value<int>());
}

LeftShiftToken::LeftShiftToken() :
    BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::LeftShift)
{
}

const ValueToken LeftShiftToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() << value2.getValue().value<int>());
}

RightShiftToken::RightShiftToken() :
    BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::RightShift)
{
}

const ValueToken RightShiftToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() >> value2.getValue().value<int>());
}

TernaryOperationToken::TernaryOperationToken(const TernaryOperationToken::OperationTypes opType) :
    OperationToken(OperationToken::Ternary),
    opType(opType)
{
}

TernaryOperationToken::OperationTypes TernaryOperationToken::getOpType() const
{
    return opType;
}

ConditionalToken::ConditionalToken() :
    TernaryOperationToken(TernaryOperationToken::Conditional)
{
}

const ValueToken ConditionalToken::DoOperation(const ValueToken &value1, const ValueToken &value2, const ValueToken &value3) const
{
    //ToThink is this really needed??
    if(value1.getValueType() != ValueToken::Bool)
        return ValueToken();

    return (value1.getValue().value<bool>()) ? ValueToken(value2) : ValueToken(value3);
}
