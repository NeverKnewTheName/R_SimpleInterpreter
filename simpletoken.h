#ifndef SIMPLETOKEN_H
#define SIMPLETOKEN_H

#include<QVariant>


class SimpleToken
{
public:
    typedef enum _TokenType
    {
        Value,
        Data,
        UnaryOperation,
        BinaryOperation,
        EOFToken
    }TokenType;
    SimpleToken(TokenType type);

private:
    const TokenType type;
};

class ValueToken : public SimpleToken
{
public:
    typedef enum _ValueTypes
    {
        Integer,
        Double
    }ValueTypes;

    ValueToken(int value);
    ValueToken(double value);

    QVariant getValue() const;

    ValueTypes getValueType() const;

private:
    const QVariant value;
    const ValueTypes valueType;
};

class UnaryOperationToken : public SimpleToken
{
public:
    typedef enum _OperationTypes
    {
        Increment,
        Decrement,
        Positive,
        Negative,
        OnesComplement,
        LogicalNegation
    }OperationTypes;

    UnaryOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value) = 0;
private:
    const OperationTypes opType;
};

class IncrementToken : public UnaryOperation
{
public:
    IncrementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class DecrementToken : public UnaryOperation
{
public:
    DecrementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class PositiveToken : public UnaryOperation
{
public:
    PositiveToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class NegativeToken : public UnaryOperation
{
public:
    NegativeToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class OnesComplementToken : public UnaryOperation
{
public:
    OnesComplementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class LogicalNegation : public UnaryOperation
{
public:
    LogicalNegation();

    virtual ValueToken DoOperation(ValueToken value);
};

class BinaryOperationToken : public SimpleToken
{
public:
    typedef enum _OperationTypes
    {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        RightShift,
        LeftShift,
        AND,
        OR,
        XOR
    }OperationTypes;

    BinaryOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2) = 0;
private:
    const OperationTypes opType;
};

class AdditionToken : public BinaryOperationToken
{
public:
    AdditionToken();

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2);
};

#endif // SIMPLETOKEN_H
