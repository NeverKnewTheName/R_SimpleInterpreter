#include "simpletoken.h"

#include <QDebug>

SimpleToken::SimpleToken() :
    type(SimpleToken::EOFToken)
{
}

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

ValueToken::ValueToken(ValueToken &&valueTokenToCopy) :
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

ValueToken &ValueToken::operator=(const ValueToken &value)
{
    valueType = value.getValueType();
    this->value = value.getValue();
    return *this;
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

OperationToken::OperationToken() :
    SimpleToken(SimpleToken::Operation)
{
}

UnaryOperationToken::UnaryOperationToken()
{

}

UnaryArithmeticOperationToken::UnaryArithmeticOperationToken()
{
}

OperationToken::OperationTypes UnaryArithmeticOperationToken::getOpType() const
{
    return OperationToken::Arithmetic;
}

IncrementToken::IncrementToken()
{
}

OperationToken::Operation IncrementToken::getOp() const
{
    return OperationToken::Increment;
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

QString IncrementToken::printValue() const
{
    return QString("++");
}

QString IncrementToken::printToken() const
{
    QString tokenType = "IncrementToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

DecrementToken::DecrementToken()
{
}

OperationToken::Operation DecrementToken::getOp() const
{
    return OperationToken::Decrement;
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

QString DecrementToken::printValue() const
{
    return QString("--");
}

QString DecrementToken::printToken() const
{
    QString tokenType = "DecrementToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

PositiveToken::PositiveToken()
{
}

OperationToken::Operation PositiveToken::getOp() const
{
    return OperationToken::Positive;
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

QString PositiveToken::printValue() const
{
    return QString("+");
}

QString PositiveToken::printToken() const
{
    QString tokenType = "PositiveToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

NegativeToken::NegativeToken()
{
}

OperationToken::Operation NegativeToken::getOp() const
{
    return OperationToken::Negative;
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

QString NegativeToken::printValue() const
{
    return QString("-");
}

QString NegativeToken::printToken() const
{
    QString tokenType = "NegativeToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

UnaryLogicalOperationToken::UnaryLogicalOperationToken()
{
}

OperationToken::OperationTypes UnaryLogicalOperationToken::getOpType() const
{
    return OperationToken::Logical;
}

LogicalNegationToken::LogicalNegationToken()
{
}

OperationToken::Operation LogicalNegationToken::getOp() const
{
    return OperationToken::LogicalNegation;
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

QString LogicalNegationToken::printValue() const
{
    return QString("!");
}

QString LogicalNegationToken::printToken() const
{
    QString tokenType = "LogicalNegationToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

UnaryBitwiseOperationToken::UnaryBitwiseOperationToken()
{
}

OperationToken::OperationTypes UnaryBitwiseOperationToken::getOpType() const
{
    return OperationToken::Bitwise;
}

OnesComplementToken::OnesComplementToken()
{
}

OperationToken::Operation OnesComplementToken::getOp() const
{
    return OperationToken::OnesComplement;
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

QString OnesComplementToken::printValue() const
{
    return QString("~");
}

QString OnesComplementToken::printToken() const
{
    QString tokenType = "OnesComplementToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

BinaryOperationToken::BinaryOperationToken()
{
}

OperationToken::ArityTypes BinaryOperationToken::getArityType() const
{
    return OperationToken::Binary;
}

BinaryArithmeticOperationToken::BinaryArithmeticOperationToken()
{
}

OperationToken::OperationTypes BinaryArithmeticOperationToken::getOpType() const
{
    return OperationToken::Arithmetic;
}

AdditionToken::AdditionToken()
{
}

OperationToken::Operation AdditionToken::getOp() const
{
    return OperationToken::Addition;
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

QString AdditionToken::printValue() const
{
    return QString("+");
}

QString AdditionToken::printToken() const
{
    QString tokenType = "AdditionToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

SubtractionToken::SubtractionToken()
{
}

OperationToken::Operation SubtractionToken::getOp() const
{
    return OperationToken::Subtraction;
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

QString SubtractionToken::printValue() const
{
    return QString("-");
}

QString SubtractionToken::printToken() const
{
    QString tokenType = "SubtractionToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

MultiplicationToken::MultiplicationToken()
{
}

OperationToken::Operation MultiplicationToken::getOp() const
{
    return OperationToken::Multiplication;
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

QString MultiplicationToken::printValue() const
{
    return QString("*");
}

QString MultiplicationToken::printToken() const
{
    QString tokenType = "MultiplicationToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

DivisionToken::DivisionToken()
{
}

OperationToken::Operation DivisionToken::getOp() const
{
    return OperationToken::Division;
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

QString DivisionToken::printValue() const
{
    return QString("/");
}

QString DivisionToken::printToken() const
{
    QString tokenType = "DivisionToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

ModuloToken::ModuloToken()
{
}

OperationToken::Operation ModuloToken::getOp() const
{
    return OperationToken::Modulo;
}

const ValueToken ModuloToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<int>() % value2.getValue().value<int>());
}

QString ModuloToken::printValue() const
{
    return QString("%");
}

QString ModuloToken::printToken() const
{
    QString tokenType = "ModuloToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

BinaryLogicalOperationToken::BinaryLogicalOperationToken()
{
}

OperationToken::OperationTypes BinaryLogicalOperationToken::getOpType() const
{
    return OperationToken::Logical;
}

LogicalANDToken::LogicalANDToken()
{
}

OperationToken::Operation LogicalANDToken::getOp() const
{
    return OperationToken::LogicalAND;
}

const ValueToken LogicalANDToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() && value2.getValue().value<bool>());
}

QString LogicalANDToken::printValue() const
{
    return QString("&&");
}

QString LogicalANDToken::printToken() const
{
    QString tokenType = "LogicalANDToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

LogicalORToken::LogicalORToken()
{
}

OperationToken::Operation LogicalORToken::getOp() const
{
    return OperationToken::LogicalOR;
}

const ValueToken LogicalORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() || value2.getValue().value<bool>());
}

QString LogicalORToken::printValue() const
{
    return QString("||");
}

QString LogicalORToken::printToken() const
{
    QString tokenType = "LogicalORToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

LogicalXORToken::LogicalXORToken()
{
}

OperationToken::Operation LogicalXORToken::getOp() const
{
    return OperationToken::LogicalXOR;
}

const ValueToken LogicalXORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return ValueToken(value1.getValue().value<bool>() != value2.getValue().value<bool>());
}

QString LogicalXORToken::printValue() const
{
    return QString("^^");
}

QString LogicalXORToken::printToken() const
{
    QString tokenType = "LogicalXORToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

GreaterToken::GreaterToken()
{
}

OperationToken::Operation GreaterToken::getOp() const
{
    return OperationToken::Greater;
}

const ValueToken GreaterToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return (value1.getValue().value<double>() > value2.getValue().value<double>()) ?  ValueToken(true) : ValueToken(false);
}

QString GreaterToken::printValue() const
{
    return QString(">");
}

QString GreaterToken::printToken() const
{
    QString tokenType = "GreaterToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

LowerToken::LowerToken()
{
}

OperationToken::Operation LowerToken::getOp() const
{
    return OperationToken::Lower;
}

const ValueToken LowerToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    return (value1.getValue().value<double>() < value2.getValue().value<double>()) ?  ValueToken(true) : ValueToken(false);
}

QString LowerToken::printValue() const
{
    return QString("<");
}

QString LowerToken::printToken() const
{
    QString tokenType = "LowerToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

EqualToken::EqualToken()
{
}

OperationToken::Operation EqualToken::getOp() const
{
    return OperationToken::Equal;
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

QString EqualToken::printValue() const
{
    return QString("==");
}

QString EqualToken::printToken() const
{
    QString tokenType = "EqualToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

EqualOrGreaterToken::EqualOrGreaterToken()
{
}

OperationToken::Operation EqualOrGreaterToken::getOp() const
{
    return OperationToken::EqualOrGreater;
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

QString EqualOrGreaterToken::printValue() const
{
    return QString(">=");
}

QString EqualOrGreaterToken::printToken() const
{
    QString tokenType = "EqualOrGreaterToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

EqualOrLowerToken::EqualOrLowerToken()
{
}

OperationToken::Operation EqualOrLowerToken::getOp() const
{
    return OperationToken::EqualOrLower;
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

QString EqualOrLowerToken::printValue() const
{
    return QString("<=");
}

QString EqualOrLowerToken::printToken() const
{
    QString tokenType = "EqualOrLowerToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

UnequalToken::UnequalToken()
{
}

OperationToken::Operation UnequalToken::getOp() const
{
    return OperationToken::Unequal;
}

const ValueToken UnequalToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
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

QString UnequalToken::printValue() const
{
    return QString("!=");
}

QString UnequalToken::printToken() const
{
    QString tokenType = "UneqalToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

BinaryBitwiseOperationToken::BinaryBitwiseOperationToken()
{
}

OperationToken::OperationTypes BinaryBitwiseOperationToken::getOpType() const
{
    return OperationToken::Bitwise;
}

ANDToken::ANDToken()
{
}

OperationToken::Operation ANDToken::getOp() const
{
    return OperationToken::AND;
}

const ValueToken ANDToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() & value2.getValue().value<int>());
}

QString ANDToken::printValue() const
{
    return QString("&");
}

QString ANDToken::printToken() const
{
    QString tokenType = "ANDToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

ORToken::ORToken()
{
}

OperationToken::Operation ORToken::getOp() const
{
    return OperationToken::OR;
}

const ValueToken ORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() | value2.getValue().value<int>());
}

QString ORToken::printValue() const
{
    return QString("|");
}

QString ORToken::printToken() const
{
    QString tokenType = "ORToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

XORToken::XORToken()
{
}

OperationToken::Operation XORToken::getOp() const
{
    return OperationToken::XOR;
}

const ValueToken XORToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() ^ value2.getValue().value<int>());
}

QString XORToken::printValue() const
{
    return QString("^");
}

QString XORToken::printToken() const
{
    QString tokenType = "XORToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

LeftShiftToken::LeftShiftToken()
{
}

OperationToken::Operation LeftShiftToken::getOp() const
{
    return OperationToken::LeftShift;
}

const ValueToken LeftShiftToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() << value2.getValue().value<int>());
}

QString LeftShiftToken::printValue() const
{
    return QString("<<");
}

QString LeftShiftToken::printToken() const
{
    QString tokenType = "LeftShiftToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

RightShiftToken::RightShiftToken()
{
}

OperationToken::Operation RightShiftToken::getOp() const
{
    return OperationToken::RightShift;
}

const ValueToken RightShiftToken::DoOperation(const ValueToken &value1, const ValueToken &value2) const
{
    //ToThink is this really needed??
    if((value1.getValueType() != ValueToken::Integer) || (value2.getValueType() != ValueToken::Integer))
        return ValueToken();

    return ValueToken(value1.getValue().value<int>() >> value2.getValue().value<int>());
}

QString RightShiftToken::printValue() const
{
    return QString(">>");
}

QString RightShiftToken::printToken() const
{
    QString tokenType = "RightShiftToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

TernaryOperationToken::TernaryOperationToken()
{
}

ConditionalToken::ConditionalToken()
{
}

OperationToken::OperationTypes ConditionalToken::getOpType() const
{
    return OperationToken::Logical;
}

OperationToken::Operation ConditionalToken::getOp() const
{
    return OperationToken::Conditional;
}

const ValueToken ConditionalToken::DoOperation(const ValueToken &value1, const ValueToken &value2, const ValueToken &value3) const
{
    //ToThink is this really needed??
    if(value1.getValueType() != ValueToken::Bool)
        return ValueToken();

    return (value1.getValue().value<bool>()) ? ValueToken(value2) : ValueToken(value3);
}

QString ConditionalToken::printValue() const
{
    return QString("?:");
}

QString ConditionalToken::printToken() const
{
    QString tokenType = "ConditionalToken";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(tokenType).arg(value);
}

OperationToken::ArityTypes UnaryOperationToken::getArityType() const
{
    return OperationToken::Unary;
}

OperationToken::ArityTypes TernaryOperationToken::getArityType() const
{
    return OperationToken::Ternary;
}
