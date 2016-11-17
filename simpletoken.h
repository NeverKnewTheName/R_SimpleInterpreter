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
        LCurlyParan,
        RParan,
        RCurlyParan,
        Data,
        Variable,
        TypeName,
        FunctionName,
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
        ReturnKeyword,
        SemiColonDelim,
        EOFToken
    }TokenType;

    SimpleToken(const int TokenPos = 0, const int TokenLen = 0);
    SimpleToken(const TokenType type, const int TokenPos = 0, const int TokenLen = 0);
    virtual ~SimpleToken();

    TokenType getTokenType() const;
    int getTokenPos() const;
    int getTokenLen() const;

    virtual QString printValue() const = 0;
    virtual QString printToken() const = 0;

    static QString convertTokenTypeToString(TokenType type);

protected:
    const TokenType type;

private:
    const int TokenPos;
    const int TokenLen;
};

typedef QSharedPointer<SimpleToken> SharedSimpleTokenPtr;

class EOFToken : public SimpleToken
{
public:
    EOFToken(const int TokenPos, const int TokenLen);
    ~EOFToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class IntegerToken : public SimpleToken
{
public:
    IntegerToken(const int value, const int TokenPos, const int TokenLen);
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
    DoubleToken(double value, const int TokenPos, const int TokenLen);
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
    BoolToken(const bool value, const int TokenPos, const int TokenLen);
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
    StringToken(const QString &value, const int TokenPos, const int TokenLen);
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
    DataToken(const unsigned int dataIndex, const int TokenPos, const int TokenLen);
    ~DataToken();

    QString printValue() const;
    QString printToken() const;

    unsigned int getDataIndex() const;

private:
    const unsigned int dataIndex;
};

class VariableToken : public SimpleToken
{
public:
    VariableToken(const QString &VariableName, const int TokenPos, const int TokenLen);
    ~VariableToken();

    QString printValue() const;
    QString printToken() const;

    QString getVariableName() const;

private:
    const QString VariableName;
};

class OperationToken : public SimpleToken
{
public:
    OperationToken(const SimpleToken::TokenType op, const int TokenPos, const int TokenLen);
    ~OperationToken();

    QString printValue() const;
    QString printToken() const;
};

class LParanToken : public OperationToken
{
public:
    LParanToken(const int TokenPos, const int TokenLen);
    ~LParanToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class RParanToken : public OperationToken
{
public:
    RParanToken(const int TokenPos, const int TokenLen);
    ~RParanToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class TypeNameToken : public SimpleToken
{
public:
    TypeNameToken(SimpleToken::TokenType type, const int TokenPos, const int TokenLen);
    ~TypeNameToken();

    SimpleToken::TokenType getType() const;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;

private:
    const SimpleToken::TokenType type;
};

//class TypeCastToken : public OperationToken
//{
//public:
//    TypeCastToken(SimpleToken::TokenType typeToCastTo, const int TokenPos, const int TokenLen);
//    ~TypeCastToken();

//    SimpleToken::TokenType getTypeToCastTo() const;

//    // SimpleToken interface
//public:
//    QString printValue() const;
//    QString printToken() const;

//private:
//    SimpleToken::TokenType typeToCastTo;
//};

#endif // SIMPLETOKEN_H
