#include "simpletoken.h"

#include <QDebug>

SimpleToken::SimpleToken(const int TokenPos, const int TokenLen) :
    type(SimpleToken::EOFToken),
    TokenPos(TokenPos),
    TokenLen(TokenLen)
{
}

SimpleToken::SimpleToken(const SimpleToken::TokenType type, const int TokenPos, const int TokenLen) :
    type(type),
    TokenPos(TokenPos),
    TokenLen(TokenLen)
{
}

SimpleToken::~SimpleToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleToken::TokenType SimpleToken::getTokenType() const
{
    return type;
}

int SimpleToken::getTokenPos() const
{
    return TokenPos;
}

int SimpleToken::getTokenLen() const
{
    return TokenLen;
}

IntegerToken::IntegerToken(int value, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::Integer, TokenPos, TokenLen),
    value(value)
{
}

IntegerToken::~IntegerToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

int IntegerToken::getValue() const
{
    return value;
}

QString IntegerToken::printValue() const
{
    return QString::number(value);
}

QString IntegerToken::printToken() const
{
    return QString("{IntegerToken}:{%1}").arg(value);
}

DoubleToken::DoubleToken(const double value, const int TokenPos, const int TokenLen ) :
    SimpleToken(SimpleToken::Double, TokenPos, TokenLen),
    value(value)
{
}

DoubleToken::~DoubleToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

double DoubleToken::getValue() const
{
    return value;
}

QString DoubleToken::printValue() const
{
    return QString::number(value);
}

QString DoubleToken::printToken() const
{
    return QString("{DoubleToken}:{%1}").arg(value);
}

BoolToken::BoolToken(bool value, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::Bool, TokenPos, TokenLen),
    value(value)
{
}

BoolToken::~BoolToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

bool BoolToken::getValue() const
{
    return value;
}

QString BoolToken::printValue() const
{
    return QString(value);
}

QString BoolToken::printToken() const
{
    return QString("{BoolToken}:{%1}").arg(value);
}

StringToken::StringToken(const QString &value, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::String, TokenPos, TokenLen),
    value(value)
{
}

StringToken::~StringToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const QString &StringToken::getValue() const
{
    return value;
}

QString StringToken::printValue() const
{
    return QString(value);
}

QString StringToken::printToken() const
{
    return QString("{StringToken}:{%1}").arg(value);
}

DataToken::DataToken(const unsigned int dataIndex, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::Data, TokenPos, TokenLen),
    dataIndex(dataIndex)
{
}

DataToken::~DataToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString DataToken::printValue() const
{
    return QString("D%1").arg(dataIndex);
}

QString DataToken::printToken() const
{
    return QString("{DataToken}:{D%1}").arg(dataIndex);
}

unsigned int DataToken::getDataIndex() const
{
    return dataIndex;
}

VariableToken::VariableToken(const QString &VariableName, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::Variable, TokenPos, TokenLen),
    VariableName(VariableName)
{

}

VariableToken::~VariableToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString VariableToken::printValue() const
{
    return QString(VariableName);
}

QString VariableToken::printToken() const
{
    return QString("{VariableToken}:{%1}").arg(VariableName);
}

QString &VariableToken::getVariableName() const
{
    return VariableName;
}

OperationToken::OperationToken(const SimpleToken::TokenType op, const int TokenPos, const int TokenLen) :
    SimpleToken(op, TokenPos, TokenLen)
{

}

OperationToken::~OperationToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString OperationToken::printValue() const
{
    return QString(type);
}

QString OperationToken::printToken() const
{
    return QString("{OperationToken}:{%1}").arg(type);
}

EOFToken::EOFToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::EOFToken, TokenPos, TokenLen)
{

}

EOFToken::~EOFToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString EOFToken::printValue() const
{
    return QString("EOF");
}

QString EOFToken::printToken() const
{
    return QString("{EOFToken}:{EOF}");
}

LParanToken::LParanToken(const int TokenPos, const int TokenLen) :
    OperationToken(SimpleToken::LParan, TokenPos, TokenLen)
{

}

LParanToken::~LParanToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString LParanToken::printValue() const
{
    return QString("(");
}

QString LParanToken::printToken() const
{
    return QString("{LParanToken}:{(}");
}

RParanToken::RParanToken(const int TokenPos, const int TokenLen) :
    OperationToken(SimpleToken::RParan, TokenPos, TokenLen)
{

}

RParanToken::~RParanToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString RParanToken::printValue() const
{
    return QString(")");
}

QString RParanToken::printToken() const
{
    return QString("{RParanToken}:{)}");
}

TypeCastToken::TypeCastToken(SimpleToken::TokenType typeToCastTo, const int TokenPos, const int TokenLen) :
    OperationToken(SimpleToken::TypeCast, TokenPos, TokenLen),
    typeToCastTo(typeToCastTo)
{
}

TypeCastToken::~TypeCastToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleToken::TokenType TypeCastToken::getTypeToCastTo() const
{
    return typeToCastTo;
}

QString TypeCastToken::printValue() const
{
    QString typeRealName;
    switch(typeToCastTo)
    {
    case SimpleToken::Integer:
        typeRealName = QString("(Integer)");
        break;
    case SimpleToken::Double:
        typeRealName = QString("(Double)");
        break;
    case SimpleToken::Bool:
        typeRealName = QString("(Bool)");
        break;
    case SimpleToken::String:
        typeRealName = QString("(String)");
        break;
    default:
        typeRealName = QString("(ERROR)");
        break;
    }

    return QString("(%1)").arg(typeRealName);
}

QString TypeCastToken::printToken() const
{
    return QString("{TypeCastToken}:{%1}").arg(printValue());
}

