#include "simpletoken.h"

#include <QDebug>

SimpleToken::SimpleToken(TokenType type) :
    type(type)
{

}

ValueToken::ValueToken() :
    SimpleToken(Value),
    valueType(ValueToken::ErrorType),
    value(QVariant())
{
}

ValueToken::ValueToken(int value) :
    SimpleToken(Value),
    valueType(ValueToken::Integer),
    value(QVariant::fromValue(value))
{
}

ValueToken::ValueToken(double value) :
    SimpleToken(Value),
    valueType(ValueToken::Double),
    value(QVariant::fromValue(value))
{
}

ValueToken::ValueToken(bool value) :
    SimpleToken(Value),
    valueType(ValueToken::Bool),
    value(QVariant::fromValue(value))
{
}

QVariant ValueToken::getValue() const
{
    return value;
}

ValueToken::ValueTypes ValueToken::getValueType() const
{
    return valueType;
}

UnaryOperationToken::UnaryOperationToken(UnaryOperationToken::OperationTypes opType) :
    SimpleToken(SimpleToken::UnaryOperation),
    opType(opType)
{

}

UnaryArithmeticOperationToken::UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::OperationTypes opType) :
    UnaryOperationToken(UnaryOperationToken::Arithmetic),
    opType(opType)
{

}

IncrementToken::IncrementToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Increment)
{

}

ValueToken IncrementToken::DoOperation(ValueToken value)
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>() + 1);
        break;
    case ValueToken::Double:
        return ValueToken(value.getValue().value<double>() + 1);
        break;
    default:
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

DecrementToken::DecrementToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Decrement)
{

}

ValueToken DecrementToken::DoOperation(ValueToken value)
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value.getValue().value<int>() - 1);
        break;
    case ValueToken::Double:
        return ValueToken(value.getValue().value<double>() - 1);
        break;
    default:
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

PositiveToken::PositiveToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Positive)
{

}

ValueToken PositiveToken::DoOperation(ValueToken value)
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
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

NegativeToken::NegativeToken() :
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::Negative)
{

}

ValueToken NegativeToken::DoOperation(ValueToken value)
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
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

UnaryBitwiseOperationToken::UnaryBitwiseOperationToken(UnaryBitwiseOperationToken::OperationTypes opType) :
    UnaryOperationToken(UnaryOperationToken::Bitwise),
    opType(opType)
{

}

OnesComplementToken::OnesComplementToken() :
    UnaryBitwiseOperationToken(UnaryBitwiseOperationToken::OnesComplement)
{

}

ValueToken OnesComplementToken::DoOperation(ValueToken value)
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(~value.getValue().value<int>());
        break;
    default:
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

LogicalNegationToken::LogicalNegationToken() :
    UnaryBitwiseOperationToken(UnaryBitwiseOperationToken::LogicalNegation)
{

}

ValueToken LogicalNegationToken::DoOperation(ValueToken value)
{
    switch(value.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(!value.getValue().value<int>());
        break;
    case ValueToken::Double:
        return ValueToken(!value.getValue().value<double>());
        break;
    default:
        qDebug() << "ERROR" << "Operation on Type " << value.getValueType() << " not supported";
    }
}

BinaryOperationToken::BinaryOperationToken(BinaryOperationToken::OperationTypes opType) :
    SimpleToken(SimpleToken::BinaryOperation),
    opType(opType)
{

}

BinaryArithmeticOperationToken::BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::OperationTypes opType) :
    BinaryOperationToken(BinaryOperationToken::Arithmetic),
    opType(opType)
{

}

AdditionToken::AdditionToken() :
    BinaryArithmeticOperationToken(BinaryArithmeticOperationToken::Addition)
{

}

BinaryLogicalOperationToken::BinaryLogicalOperationToken(BinaryLogicalOperationToken::OperationTypes opType) :
    BinaryOperationToken(BinaryOperationToken::Logical),
    opType(opType)
{

}

BinaryBitwiseOperationToken::BinaryBitwiseOperationToken(BinaryBitwiseOperationToken::OperationTypes opType) :
    BinaryOperationToken(BinaryOperationToken::Bitwise),
    opType(opType)
{

}

ValueToken AdditionToken::DoOperation(ValueToken value1, ValueToken value2)
{
    switch(value1.getValueType())
    {
    case ValueToken::Integer:
        return ValueToken(value1.getValue().value<int>() + value2.getValue().value<int>() );
        break;
    case ValueToken::Double:
        return ValueToken(value1.getValue().value<double>() + value2.getValue().value<double>() );
        break;
    default:
        qDebug() << "ERROR" << "Operation on Type " << value1.getValueType() << " not supported";
    }
}

TernaryOperationToken::TernaryOperationToken(TernaryOperationToken::OperationTypes opType) :
    SimpleToken(TernaryOperation),
    opType(opType)
{

}

ConditionalToken::ConditionalToken() :
    TernaryOperationToken(TernaryOperationToken::Conditional)
{

}

ValueToken ConditionalToken::DoOperation(ValueToken value1, ValueToken value2, ValueToken value3)
{
    if(value1.getValueType() != ValueToken::Bool)
    {
        qDebug() << "ERROR" << "Value1 should be bool but is " << value1.getValueType();
        return ValueToken();
    }
    if(value1.getValue().value<bool>())
    {
        return value2;
    }
    else
    {
        return value3;
    }
}

DataToken::DataToken(unsigned int dataIndex) :
    SimpleToken(Data),
    dataIndex(dataIndex)
{

}
