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
    SimpleToken();
    SimpleToken(const TokenType type);

    TokenType getTokenType() const;
    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;

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
    ValueToken(const ValueToken &valueTokenToCopy);
    ValueToken(ValueToken &&valueTokenToCopy);
    ValueToken(const int value);
    ValueToken(const double value);
    ValueToken(const bool value);
    ValueToken(QString const &value);

    ValueToken &operator=(ValueToken const&value);

    const QVariant getValue() const;

    ValueTypes getValueType() const;

    virtual QString printValue() const;
    virtual QString printToken() const;

private:
    ValueTypes valueType;
    QVariant value;
};

class DataToken : public SimpleToken
{
public:
    DataToken(const unsigned int dataIndex);

    const ValueToken getData(QVector<quint8> const &dataVector) const;

    virtual QString printValue() const;
    virtual QString printToken() const;

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
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;
    typedef enum _Operation
    {
        Increment,
        Decrement,
        Positive,
        Negative,
        LogicalNegation,
        OnesComplement,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        LogicalAND,
        LogicalOR,
        LogicalXOR,
        Greater,
        Lower,
        Equal,
        EqualOrGreater,
        EqualOrLower,
        Unequal,
        AND,
        OR,
        XOR,
        LeftShift,
        RightShift,
        Conditional
    }Operation;

    OperationToken();
    virtual ArityTypes getArityType() const = 0;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class UnaryOperationToken : public OperationToken
{
public:
    UnaryOperationToken();
    virtual ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class UnaryArithmeticOperationToken : public UnaryOperationToken
{
public:
    UnaryArithmeticOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class IncrementToken : public UnaryArithmeticOperationToken
{
public:
    IncrementToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class DecrementToken : public UnaryArithmeticOperationToken
{
public:
    DecrementToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class PositiveToken : public UnaryArithmeticOperationToken
{
public:
    PositiveToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class NegativeToken : public UnaryArithmeticOperationToken
{
public:
    NegativeToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class UnaryLogicalOperationToken : public UnaryOperationToken
{
public:
    UnaryLogicalOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class LogicalNegationToken : public UnaryLogicalOperationToken
{
public:
    LogicalNegationToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class UnaryBitwiseOperationToken : public UnaryOperationToken
{
public:
    UnaryBitwiseOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class OnesComplementToken : public UnaryBitwiseOperationToken
{
public:
    OnesComplementToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class BinaryOperationToken : public OperationToken
{
public:
    BinaryOperationToken();
    virtual ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class BinaryArithmeticOperationToken : public BinaryOperationToken
{
public:
    BinaryArithmeticOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class AdditionToken : public BinaryArithmeticOperationToken
{
public:
    AdditionToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class SubtractionToken : public BinaryArithmeticOperationToken
{
public:
    SubtractionToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class MultiplicationToken : public BinaryArithmeticOperationToken
{
public:
    MultiplicationToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class DivisionToken : public BinaryArithmeticOperationToken
{
public:
    DivisionToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class ModuloToken : public BinaryArithmeticOperationToken
{
public:
    ModuloToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class BinaryLogicalOperationToken : public BinaryOperationToken
{
public:
    BinaryLogicalOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class LogicalANDToken : public BinaryLogicalOperationToken
{
public:
    LogicalANDToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class LogicalORToken : public BinaryLogicalOperationToken
{
public:
    LogicalORToken();
    virtual Operation getOp() const;

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

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class LogicalXORToken : public BinaryLogicalOperationToken
{
public:
    LogicalXORToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class GreaterToken : public BinaryLogicalOperationToken
{
public:
    GreaterToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class LowerToken : public BinaryLogicalOperationToken
{
public:
    LowerToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class EqualToken : public BinaryLogicalOperationToken
{
public:
    EqualToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class EqualOrGreaterToken : public BinaryLogicalOperationToken
{
public:
    EqualOrGreaterToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class EqualOrLowerToken : public BinaryLogicalOperationToken
{
public:
    EqualOrLowerToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class UnequalToken : public BinaryLogicalOperationToken
{
public:
    UnequalToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class BinaryBitwiseOperationToken : public BinaryOperationToken
{
public:

    BinaryBitwiseOperationToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class ANDToken : public BinaryBitwiseOperationToken
{
public:
    ANDToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class ORToken : public BinaryBitwiseOperationToken
{
public:
    ORToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class XORToken : public BinaryBitwiseOperationToken
{
public:
    XORToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class LeftShiftToken : public BinaryBitwiseOperationToken
{
public:
    LeftShiftToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class RightShiftToken : public BinaryBitwiseOperationToken
{
public:
    RightShiftToken();
    virtual Operation getOp() const;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

class TernaryOperationToken : public OperationToken
{
public:

    TernaryOperationToken();
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;

    virtual const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2, ValueToken const &value3) const = 0;
    virtual ArityTypes getArityType() const;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;
};

class ConditionalToken : public TernaryOperationToken
{
public:
    ConditionalToken();
    virtual OperationTypes getOpType() const;
    virtual Operation getOp() const;

    const ValueToken DoOperation(ValueToken const &value1, ValueToken const &value2, ValueToken const &value3) const;

    virtual QString printValue() const;
    virtual QString printToken() const;
};

#endif // SIMPLETOKEN_H
