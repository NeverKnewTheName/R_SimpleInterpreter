#ifndef SIMPLETOKEN_H
#define SIMPLETOKEN_H

#include <QVariant>
#include <QSharedPointer>

class SimpleToken
{
public:
    typedef enum _TokenType
    {
        Integer,
        Double,
        Bool,
        String,
        LParan,
        RParan,
        Data,
        Symbol,
        Increment,
        Decrement,
        Plus,
        Minus,
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
    virtual ~SimpleToken();

    TokenType getTokenType() const;
    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;

protected:
    const TokenType type;
};

typedef QSharedPointer<SimpleToken> SharedSimpleTokenPtr;

class EOFToken : public SimpleToken
{
public:
    EOFToken();
    ~EOFToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class IntegerToken : public SimpleToken
{
public:
    IntegerToken(int value);
    ~IntegerToken();
    int getValue() const;

private:
    const int value;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class DoubleToken : public SimpleToken
{
public:
    DoubleToken(double value);
    ~DoubleToken();
    double getValue() const;

private:
    const double value;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class BoolToken : public SimpleToken
{
public:
    BoolToken(bool value);
    ~BoolToken();
    bool getValue() const;

private:
    const bool value;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class StringToken : public SimpleToken
{
public:
    StringToken(QString &value);
    ~StringToken();
    QString const &getValue() const;

private:
    const QString value;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class DataToken : public SimpleToken
{
public:
    DataToken(const unsigned int dataIndex);
    ~DataToken();

    QString printValue() const;
    QString printToken() const;

    unsigned int getDataIndex() const;

private:
    const unsigned int dataIndex;
};

class OperationToken : public SimpleToken
{
public:
    OperationToken(const SimpleToken::TokenType op);
    ~OperationToken();

    QString printValue() const;
    QString printToken() const;
};

class LParanToken : public OperationToken
{
public:
    LParanToken();
    ~LParanToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class RParanToken : public OperationToken
{
public:
    RParanToken();
    ~RParanToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

#endif // SIMPLETOKEN_H
