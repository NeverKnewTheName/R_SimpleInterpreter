#ifndef SIMPLETOKEN_H
#define SIMPLETOKEN_H

#include<QVariant>

class SimpleToken
{
public:
    typedef enum _TokenType
    {
        Integer,
        Double,
        Bool,
        String,
        Data,
        Symbol,
        Increment,
        Decrement,
        Plus,
        Ninus,
        LogicalNegation,
        OnesComplement,
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
        BitwiseAND,
        BitwiseOR,
        BitwiseXOR,
        LeftShift,
        RightShift,
        Conditional,
        EOFToken
    }TokenType;

    SimpleToken();
    SimpleToken(const TokenType type);

    TokenType getTokenType() const;
    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;

protected:
    const TokenType type;
};

class IntegerToken : public SimpleToken
{
public:
    IntegerToken(int value);
    int getValue() const;

private:
    const int value;

    // SimpleToken interface
public:
    virtual QString printValue() const;
    virtual QString printToken() const;
};

class DoubleToken : public SimpleToken
{
public:
    DoubleToken(double value);
    double getValue() const;

private:
    const double value;

    // SimpleToken interface
public:
    virtual QString printValue() const;
    virtual QString printToken() const;
};

class BoolToken : public SimpleToken
{
public:
    BoolToken(bool value);
    bool getValue() const;

private:
    const bool value;

    // SimpleToken interface
public:
    virtual QString printValue() const;
    virtual QString printToken() const;
};

class StringToken : public SimpleToken
{
public:
    StringToken(QString &value);
    QString const &getValue() const;

private:
    const QString value;

    // SimpleToken interface
public:
    virtual QString printValue() const;
    virtual QString printToken() const;
};

class DataToken : public SimpleToken
{
public:
    DataToken(const unsigned int dataIndex);

    virtual QString printValue() const;
    virtual QString printToken() const;

    unsigned int getDataIndex() const;

private:
    const unsigned int dataIndex;
};

class OperationToken : public SimpleToken
{
public:
    OperationToken(const SimpleToken::TokenType op);

    virtual QString printValue() const;
    virtual QString printToken() const;
};

#endif // SIMPLETOKEN_H
