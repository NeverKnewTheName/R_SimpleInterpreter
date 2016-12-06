#ifndef SIMPLETOKEN_H
#define SIMPLETOKEN_H

#include <QVariant>
#include <QSharedPointer>
#include <QDebug>

#include "simplenode.h"

class SimpleToken
{
public:
    typedef enum _TokenType
    {
//        Integer,
//        Double,
//        Bool,
//        String,
//        Void,
        Value,
        LParan,
        LCurlyParan,
        RParan,
        RCurlyParan,
        Data,
        VariableID,
        TypeName,
        Assign,
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
        QMark,
        Colon,
        CommaDelim,
        SemiColonDelim,
        If,
        Else,
        Switch,
        Case,
        For,
        While,
        Do,
        Break,
        Continue,
        ReturnKeyword,
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

class ValueToken : public SimpleToken
{
public:
    ValueToken(const QVariant value, const Node::ValueTypes valueType, const int TokenPos, const int TokenLen) :
        SimpleToken(SimpleToken::Value, TokenPos, TokenLen),
        value(value),
        valueType(valueType)
    {
        qDebug() << "ValueToken - Value: " << value << " Type: " << valueType;
    }

    ~ValueToken()
    {
        qDebug() << __PRETTY_FUNCTION__;
    }

    int getInt() const
    {
        return value.value<int>();
    }
    double getDouble() const
    {
        return value.value<double>();
    }
    bool getBool() const
    {
        return value.value<bool>();
    }
    QString getString() const
    {
        return value.value<QString>();
    }

    Node::ValueTypes getValueType() const
    {
        return valueType;
    }

    // SimpleToken interface
public:
    QString printValue() const
    {
        return QString("%1").arg(value.value<QString>());
    }
    QString printToken() const
    {
        return QString("{ValueToken(%1)}:{%2}").arg(SimpleNode::getHumanReadableTypeNameToValueType(valueType)).arg(printValue());
    }

private:
    const QVariant value;
    const Node::ValueTypes valueType;
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

class VariableIDToken : public SimpleToken
{
public:
    VariableIDToken(const QString &ID, const int TokenPos, const int TokenLen);
    ~VariableIDToken();

    QString printValue() const;
    QString printToken() const;

    QString getID() const;

private:
    const QString ID;
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
class LCurlyParanToken : public OperationToken
{
public:
    LCurlyParanToken(const int TokenPos, const int TokenLen);
    ~LCurlyParanToken();

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

class RCurlyParanToken : public OperationToken
{
public:
    RCurlyParanToken(const int TokenPos, const int TokenLen);
    ~RCurlyParanToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class TypeNameToken : public SimpleToken
{
public:
    TypeNameToken(Node::ValueTypes type, const int TokenPos, const int TokenLen);
    ~TypeNameToken();

    Node::ValueTypes getType() const;

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;

private:
    const Node::ValueTypes type;
};

class CommaDelimToken : public SimpleToken
{
public:
    CommaDelimToken(const int TokenPos, const int TokenLen);
    ~CommaDelimToken();


    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class SemiColonDelimToken : public SimpleToken
{
public:
    SemiColonDelimToken(const int TokenPos, const int TokenLen);
    ~SemiColonDelimToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class ReturnKeywordToken : public SimpleToken
{
public:
    ReturnKeywordToken(const int TokenPos, const int TokenLen);
    ~ReturnKeywordToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class QMarkToken : public SimpleToken
{
public:
    QMarkToken(const int TokenPos, const int TokenLen);
    ~QMarkToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class ColonToken : public SimpleToken
{
public:
    ColonToken(const int TokenPos, const int TokenLen);
    ~ColonToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

class ControlToken : public SimpleToken
{
public:
    ControlToken(SimpleToken::TokenType type, const int TokenPos, const int TokenLen);
    ~ControlToken();

    // SimpleToken interface
public:
    QString printValue() const;
    QString printToken() const;
};

#endif // SIMPLETOKEN_H
