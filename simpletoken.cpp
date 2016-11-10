#include "simpletoken.h"

SimpleToken::SimpleToken(TokenType type) :
    type(type)
{

}

ValueToken::ValueToken(int value) :
    SimpleToken(Value),
    valueType(Integer),
    value(QVariant::fromValue(value))
{
//    this->value = QVariant::fromValue(value);
}

ValueToken::ValueToken(double value) :
    SimpleToken(Value),
    valueType(Double),
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

BinaryOperationToken::BinaryOperationToken(BinaryOperationToken::OperationTypes opType) :
    SimpleToken(SimpleToken::BinaryOperation),
    opType(opType)
{

}

IncrementToken::IncrementToken() :
    UnaryOperationToken(UnaryOperationToken::Increment)
{

}

ValueToken IncrementToken::DoOperation(ValueToken value)
{
    return ValueToken(value.getValue()++);
}

DecrementToken::DecrementToken() :
    UnaryOperationToken(UnaryOperationToken::Decrement)
{

}

ValueToken DecrementToken::DoOperation(ValueToken value)
{
    return ValueToken(value.getValue()--);
}

PositiveToken::PositiveToken() :
    UnaryOperationToken(UnaryOperationToken::Positive)
{

}

ValueToken PositiveToken::DoOperation(ValueToken value)
{
    return ValueToken(value.getValue());
}

NegativeToken::NegativeToken() :
    UnaryOperationToken(UnaryOperationToken::Negative)
{

}

ValueToken NegativeToken::DoOperation(ValueToken value)
{
    return ValueToken(value.getValue() * -1);
}

OnesComplementToken::OnesComplementToken() :
    UnaryOperationToken(UnaryOperationToken::OnesComplement)
{

}

ValueToken OnesComplementToken::DoOperation(ValueToken value)
{
}

AdditionToken::AdditionToken() :
    BinaryOperationToken(BinaryOperationToken::Addition)
{

}

ValueToken AdditionToken::DoOperation(ValueToken value1, ValueToken value2)
{

}
