#include "simpletoken.h"

#include <QDebug>

SimpleToken::SimpleToken() :
    type(SimpleToken::EOFToken)
{
}

SimpleToken::SimpleToken(const SimpleToken::TokenType type) :
    type(type)
{
}

SimpleToken::TokenType SimpleToken::getTokenType() const
{
    return type;
}

IntegerToken::IntegerToken(int value) :
    value(value)
{
}

int IntegerToken::getValue() const
{
    return value;
}

QString IntegerToken::printValue() const
{
    return QString(value);
}

QString IntegerToken::printToken() const
{
    return QString("{IntegerToken}:{%1}").arg(value);
}

DoubleToken::DoubleToken(double value) :
    value(value)
{
}

double DoubleToken::getValue() const
{
    return value;
}

QString DoubleToken::printValue() const
{
    return QString(value);
}

QString DoubleToken::printToken() const
{
    return QString("{DoubleToken}:{%1}").arg(value);
}

BoolToken::BoolToken(bool value) :
    value(value)
{
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

StringToken::StringToken(QString &value) :
    value(value)
{
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

DataToken::DataToken(const unsigned int dataIndex) :
    dataIndex(dataIndex)
{
}

QString DataToken::printValue() const
{
    return QString("D%1").arg(dataIndex);
}

QString DataToken::printToken() const
{
    return QString("{DataToken}:{D%1}").arg(dataIndex);
}

OperationToken::OperationToken(const SimpleToken::TokenType op) :
    SimpleToken(op)
{

}

QString OperationToken::printValue() const
{
    return QString(op);
}

QString OperationToken::printToken() const
{
    return QString("{OperationToken}:{%1}").arg(op);
}
