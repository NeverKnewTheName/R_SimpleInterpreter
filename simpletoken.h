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
        Operation,
        EOFToken
    }TokenType;
    SimpleToken(const TokenType type);

    TokenType getTokenType() const;

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
        String,
        ErrorType
    }ValueTypes;

    ValueToken();
    ValueToken(ValueToken const &valueTokenToCopy);
    ValueToken(const int value);
    ValueToken(const double value);
    ValueToken(const bool value);
    ValueToken(QString const &value);

    const QVariant getValue() const;

    ValueTypes getValueType() const;

    QString printValue() const;
    QString printToken() const;

private:
    const ValueTypes valueType;
    const QVariant value;
};

class DataToken : public SimpleToken
{
public:
    DataToken(const unsigned int dataIndex);

    const ValueToken getData(QVector<quint8> const &dataVector) const;
    QString printValue() const;
    QString printToken() const;
private:
    const unsigned int dataIndex;
};

class OperationToken : public SimpleToken
{
public:
    typedef enum _ArityTypes
    {
        Unary,
        Binary,
        Ternary
    }ArityTypes;

    OperationToken(const ArityTypes arityType);
    ArityTypes getArityType() const;

private:
    const ArityTypes arityType;
};

class UnaryOperationToken : public OperationToken
{
public:
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;

    UnaryOperationToken(const OperationTypes opType);

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;
    OperationTypes getOpType() const;
private:
    const OperationTypes opType;
};

class UnaryArithmeticOperationToken : public UnaryOperationToken
{
public:
    typedef enum _Operation
    {
        Increment,
        Decrement,
        Positive,
        Negative
    }Operation;
    UnaryArithmeticOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;
    Operation getOp() const;
private:
    const Operation op;
};

class IncrementToken : public UnaryArithmeticOperationToken
{
public:
    IncrementToken();

    /**
     * \brief Increments Integer Values by one and returns the result as a new ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Integer when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueTokens
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class DecrementToken : public UnaryArithmeticOperationToken
{
public:
    DecrementToken();

    /**
     * \brief Decrements Integer Values by one and returns the result as a new ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Integer when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueTokens
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class PositiveToken : public UnaryArithmeticOperationToken
{
public:
    PositiveToken();

    /**
     * \brief Returns a new ValueToken with the value of the passed ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Integer or Double when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer or Double ValueTokens
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class NegativeToken : public UnaryArithmeticOperationToken
{
public:
    NegativeToken();

    /**
     * \brief Returns a new ValueToken with the value of the passed ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Integer or Double when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer or Double ValueTokens
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class UnaryLogicalOperationToken : public UnaryOperationToken
{
public:
    typedef enum _Operation
    {
        LogicalNegation
    }Operation;

    UnaryLogicalOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken value) const = 0;
    Operation getOp() const;

private:
    const Operation op;
};

class LogicalNegationToken : public UnaryLogicalOperationToken
{
public:
    LogicalNegationToken();

    /**
     * \brief Returns a new ValueToken with the logically negated value of the passed ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Bool when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer, Double or Bool ValueTokens
     * \note If the passed ValueToken equals 0 (or 0.0) it is treated as Bool false, otherwise Bool true
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class UnaryBitwiseOperationToken : public UnaryOperationToken
{
public:
    typedef enum _Operation
    {
        OnesComplement
    }Operation;
    UnaryBitwiseOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;

    Operation getOp() const;

private:
    const Operation op;
};

class OnesComplementToken : public UnaryBitwiseOperationToken
{
public:
    OnesComplementToken();

    /**
     * \brief Returns a new ValueToken with the bitwise one's complement of the passed ValueToken
     *
     * \param value[in] value to applay the operation to
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Integer when successful
     * \returns ValueToken of type ErrorType when failed
     *
     * \warning Only use this operation on Integer ValueTokens
     */
    virtual const ValueToken DoOperation(ValueToken const &value) const;
};

class BinaryOperationToken : public OperationToken
{
public:
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;

    BinaryOperationToken(const OperationTypes opType);

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;
    OperationTypes getOpType() const;

private:
    const OperationTypes opType;
};

class BinaryArithmeticOperationToken : public BinaryOperationToken
{
public:
    typedef enum _Operation
    {
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo
    }Operation;

    BinaryArithmeticOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;
    Operation getOp() const;

private:
    const Operation op;
};

class AdditionToken : public BinaryArithmeticOperationToken
{
public:
    AdditionToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class SubtractionToken : public BinaryArithmeticOperationToken
{
public:
    SubtractionToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class MultiplicationToken : public BinaryArithmeticOperationToken
{
public:
    MultiplicationToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class DivisionToken : public BinaryArithmeticOperationToken
{
public:
    DivisionToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class ModuloToken : public BinaryArithmeticOperationToken
{
public:
    ModuloToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class BinaryLogicalOperationToken : public BinaryOperationToken
{
public:
    typedef enum _Operation
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
    }Operation;

    BinaryLogicalOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;
    Operation getOpType() const;

private:
    const Operation op;
};

class LogicalANDToken : public BinaryLogicalOperationToken
{
public:
    LogicalANDToken();

    /**
     * \brief Returns a new ValueToken of type Bool with the result of the AND operation
     *
     * \param value1[in] first operand
     * \param value2[in] second operand
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Bool when successful
     *
     * \warning Do not use on type String
     * \note Integer and Double ValueTokens are converted to bool before the operation takes place (0 or 0.0 is false whereas everything else  is true)
     */
    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class LogicalORToken : public BinaryLogicalOperationToken
{
public:
    LogicalORToken();

    /**
     * \brief Returns a new ValueToken of type Bool with the result of the OR operation
     *
     * \param value1[in] first operand
     * \param value2[in] second operand
     *
     * \return ValueToken with the operation's result
     * \returns ValueToken of type Bool when successful
     *
     * \warning Do not use on type String
     * \note Integer and Double ValueTokens are converted to bool before the operation takes place (0 or 0.0 is false whereas everything else  is true)
     */
    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class LogicalXORToken : public BinaryLogicalOperationToken
{
public:
    LogicalXORToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class GreaterToken : public BinaryLogicalOperationToken
{
public:
    GreaterToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class LowerToken : public BinaryLogicalOperationToken
{
public:
    LowerToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class EqualToken : public BinaryLogicalOperationToken
{
public:
    EqualToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class EqualOrGreaterToken : public BinaryLogicalOperationToken
{
public:
    EqualOrGreaterToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class EqualOrLowerToken : public BinaryLogicalOperationToken
{
public:
    EqualOrLowerToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class UneqalToken : public BinaryLogicalOperationToken
{
public:
    UneqalToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class BinaryBitwiseOperationToken : public BinaryOperationToken
{
public:
    typedef enum _Operation
    {
        AND,
        OR,
        XOR,
        LeftShift,
        RightShift
    }Operation;

    BinaryBitwiseOperationToken(const Operation op);

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;
    Operation getOp() const;

private:
    const Operation op;
};

class ANDToken : public BinaryBitwiseOperationToken
{
public:
    ANDToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class ORToken : public BinaryBitwiseOperationToken
{
public:
    ORToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class XORToken : public BinaryBitwiseOperationToken
{
public:
    XORToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class LeftShiftToken : public BinaryBitwiseOperationToken
{
public:
    LeftShiftToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class RightShiftToken : public BinaryBitwiseOperationToken
{
public:
    RightShiftToken();

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;
};

class TernaryOperationToken : public OperationToken
{
public:
    typedef enum _OperationTypes
    {
        Conditional
    }OperationTypes;

    TernaryOperationToken(const OperationTypes opType);

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2, ValueToken const &value3) const = 0;
    OperationTypes getOpType() const;

private:
    const OperationTypes opType;
};

class ConditionalToken : public TernaryOperationToken
{
    ConditionalToken();

    const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2, ValueToken const &value3) const;
};

#endif // SIMPLETOKEN_H
