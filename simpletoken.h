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
        TernaryOperation,
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
        Double,
        Bool,
        ErrorType
    }ValueTypes;

    ValueToken();
    ValueToken(int value);
    ValueToken(double value);
    ValueToken(bool value);

    QVariant getValue() const;

    ValueTypes getValueType() const;

private:
    const ValueTypes valueType;
    const QVariant value;
};

class DataToken : public SimpleToken
{
public:
    DataToken(unsigned int dataIndex);

    ValueToken getData(QVector<quint8> dataVector);
private:
    unsigned int dataIndex;
};

class UnaryOperationToken : public SimpleToken
{
public:
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;

    UnaryOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value) = 0;
private:
    const OperationTypes opType;
};

class UnaryArithmeticOperationToken : public UnaryOperationToken
{
public:
    typedef enum _OperationTypes
    {
        Increment,
        Decrement,
        Positive,
        Negative
    }OperationTypes;
    UnaryArithmeticOperationToken(UnaryArithmeticOperationToken::OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value) = 0;
private:
    const OperationTypes opType;
};

class IncrementToken : public UnaryArithmeticOperationToken
{
public:
    IncrementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class DecrementToken : public UnaryArithmeticOperationToken
{
public:
    DecrementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class PositiveToken : public UnaryArithmeticOperationToken
{
public:
    PositiveToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class NegativeToken : public UnaryArithmeticOperationToken
{
public:
    NegativeToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class UnaryBitwiseOperationToken : public UnaryOperationToken
{
public:
    typedef enum _OperationTypes
    {

        OnesComplement,
        LogicalNegation
    }OperationTypes;
    UnaryBitwiseOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value) = 0;
private:
    const OperationTypes opType;
};

class OnesComplementToken : public UnaryBitwiseOperationToken
{
public:
    OnesComplementToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class LogicalNegationToken : public UnaryBitwiseOperationToken
{
public:
    LogicalNegationToken();

    virtual ValueToken DoOperation(ValueToken value);
};

class BinaryOperationToken : public SimpleToken
{
public:
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;

    BinaryOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2) = 0;
private:
    const OperationTypes opType;
};

class BinaryArithmeticOperationToken : public BinaryOperationToken
{
public:
    typedef enum _OperationTypes
    {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo
    }OperationTypes;

    BinaryArithmeticOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2) = 0;
private:
    const OperationTypes opType;
};

class AdditionToken : public BinaryArithmeticOperationToken
{
public:
    AdditionToken();

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2);
};

class BinaryLogicalOperationToken : public BinaryOperationToken
{
public:
    typedef enum _OperationTypes
    {
        AND,
        OR,
        XOR,
        Greater,
        Lower,
        Equal,
        EqualOrGreater,
        EqualOrLower,
        Unequal
    }OperationTypes;

    BinaryLogicalOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2) = 0;
private:
    const OperationTypes opType;
};

class BinaryBitwiseOperationToken : public BinaryOperationToken
{
public:
    typedef enum _OperationTypes
    {
        AND,
        OR,
        XOR
    }OperationTypes;

    BinaryBitwiseOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2) = 0;
private:
    const OperationTypes opType;
};

class TernaryOperationToken : public SimpleToken
{
public:
    typedef enum _OperationTypes
    {
        Conditional
    }OperationTypes;

    TernaryOperationToken(OperationTypes opType);

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2, ValueToken value3) = 0;
private:
    const OperationTypes opType;
};

class ConditionalToken : public TernaryOperationToken
{
    ConditionalToken();

    virtual ValueToken DoOperation(ValueToken value1, ValueToken value2, ValueToken value3);
};

#endif // SIMPLETOKEN_H
