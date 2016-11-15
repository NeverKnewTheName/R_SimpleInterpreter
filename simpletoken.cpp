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

SimpleToken::~SimpleToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleToken::TokenType SimpleToken::getTokenType() const
{
    return type;
}

IntegerToken::IntegerToken(int value) :
    SimpleToken(SimpleToken::Integer),
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

DoubleToken::DoubleToken(double value) :
    SimpleToken(SimpleToken::Double),
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

BoolToken::BoolToken(bool value) :
    SimpleToken(SimpleToken::Bool),
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

StringToken::StringToken(QString &value) :
    SimpleToken(SimpleToken::String),
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

DataToken::DataToken(const unsigned int dataIndex) :
    SimpleToken(SimpleToken::Data),
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

OperationToken::OperationToken(const SimpleToken::TokenType op) :
    SimpleToken(op)
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

EOFToken::EOFToken() :
    SimpleToken(SimpleToken::EOFToken)
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

LParanToken::LParanToken() :
    OperationToken(SimpleToken::LParan)
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

RParanToken::RParanToken() :
    OperationToken(SimpleToken::RParan)
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
