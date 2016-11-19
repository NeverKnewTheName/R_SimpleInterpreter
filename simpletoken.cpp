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

QString SimpleToken::convertTokenTypeToString(SimpleToken::TokenType type)
{
    QString typeString("UNDEFINED");
    switch(type)
    {
//    case Integer:
//        typeString = QString("Integer");
//        break;
//    case Double:
//        typeString = QString("Double");
//        break;
//    case Bool:
//        typeString = QString("Bool");
//        break;
//    case String:
//        typeString = QString("String");
//        break;
    case Value:
        typeString = QString("Value");
        break;
    case LParan:
        typeString = QString("LParan");
        break;
    case LCurlyParan:
        typeString = QString("LCurlyParan");
        break;
    case RParan:
        typeString = QString("RParan");
        break;
    case RCurlyParan:
        typeString = QString("RCurlyParan");
        break;
    case Data:
        typeString = QString("Data");
        break;
    case VariableID:
        typeString = QString("VariableID");
        break;
    case TypeName:
        typeString = QString("TypeName");
        break;
    case Increment:
        typeString = QString("Increment");
        break;
    case Decrement:
        typeString = QString("Decrement");
        break;
    case Plus:
        typeString = QString("Plus");
        break;
    case Minus:
        typeString = QString("Minus");
        break;
    case LogicalNegation:
        typeString = QString("LogicalNegation");
        break;
    case OnesComplement:
        typeString = QString("OnesComplement");
        break;
    case Multiplication:
        typeString = QString("Multiplication");
        break;
    case Division:
        typeString = QString("Division");
        break;
    case Modulo:
        typeString = QString("Modulo");
        break;
    case LogicalAND:
        typeString = QString("LogicalAND");
        break;
    case LogicalOR:
        typeString = QString("LogicalOR");
        break;
    case LogicalXOR:
        typeString = QString("LogicalXOR");
        break;
    case Greater:
        typeString = QString("Greater");
        break;
    case Lower:
        typeString = QString("Lower");
        break;
    case Equal:
        typeString = QString("Equal");
        break;
    case EqualOrGreater:
        typeString = QString("EqualOrGreater");
        break;
    case EqualOrLower:
        typeString = QString("EqualOrLower");
        break;
    case Unequal:
        typeString = QString("Unequal");
        break;
    case BitwiseAND:
        typeString = QString("BitwiseAND");
        break;
    case BitwiseOR:
        typeString = QString("BitwiseOR");
        break;
    case BitwiseXOR:
        typeString = QString("BitwiseXOR");
        break;
    case LeftShift:
        typeString = QString("LeftShift");
        break;
    case RightShift:
        typeString = QString("RightShift");
        break;
    case Assign:
        typeString = QString("Assign");
        break;
    case QMark:
        typeString = QString("QMark");
        break;
    case Colon:
        typeString = QString("Colon");
        break;
    case SemiColonDelim:
        typeString = QString("SemiColonDelim");
        break;
    case ReturnKeyword:
        typeString = QString("ReturnKeyword");
        break;
    case EOFToken:
        typeString = QString("EOFToken");
        break;
    }
    return typeString;
}

//IntegerToken::IntegerToken(int value, const int TokenPos, const int TokenLen) :
//    SimpleToken(SimpleToken::Integer, TokenPos, TokenLen),
//    value(value)
//{
//}

//IntegerToken::~IntegerToken()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//int IntegerToken::getValue() const
//{
//    return value;
//}

//QString IntegerToken::printValue() const
//{
//    return QString::number(value);
//}

//QString IntegerToken::printToken() const
//{
//    return QString("{IntegerToken}:{%1}").arg(value);
//}

//DoubleToken::DoubleToken(const double value, const int TokenPos, const int TokenLen ) :
//    SimpleToken(SimpleToken::Double, TokenPos, TokenLen),
//    value(value)
//{
//}

//DoubleToken::~DoubleToken()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//double DoubleToken::getValue() const
//{
//    return value;
//}

//QString DoubleToken::printValue() const
//{
//    return QString::number(value);
//}

//QString DoubleToken::printToken() const
//{
//    return QString("{DoubleToken}:{%1}").arg(value);
//}

//BoolToken::BoolToken(bool value, const int TokenPos, const int TokenLen) :
//    SimpleToken(SimpleToken::Bool, TokenPos, TokenLen),
//    value(value)
//{
//}

//BoolToken::~BoolToken()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//bool BoolToken::getValue() const
//{
//    return value;
//}

//QString BoolToken::printValue() const
//{
//    return QString(value);
//}

//QString BoolToken::printToken() const
//{
//    return QString("{BoolToken}:{%1}").arg(value);
//}

//StringToken::StringToken(const QString &value, const int TokenPos, const int TokenLen) :
//    SimpleToken(SimpleToken::String, TokenPos, TokenLen),
//    value(value)
//{
//}

//StringToken::~StringToken()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//const QString &StringToken::getValue() const
//{
//    return value;
//}

//QString StringToken::printValue() const
//{
//    return QString(value);
//}

//QString StringToken::printToken() const
//{
//    return QString("{StringToken}:{%1}").arg(value);
//}

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

VariableIDToken::VariableIDToken(const QString &ID, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::VariableID, TokenPos, TokenLen),
    ID(ID)
{

}

VariableIDToken::~VariableIDToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString VariableIDToken::printValue() const
{
    return QString(ID);
}

QString VariableIDToken::printToken() const
{
    return QString("{VariableIDToken}:{%1}").arg(ID);
}

QString VariableIDToken::getID() const
{
    return ID;
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
    return QString("{OperationToken}:{%1}").arg(SimpleToken::convertTokenTypeToString(type));
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

//TypeCastToken::TypeCastToken(SimpleToken::TokenType typeToCastTo, const int TokenPos, const int TokenLen) :
//    OperationToken(SimpleToken::TypeCast, TokenPos, TokenLen),
//    typeToCastTo(typeToCastTo)
//{
//}

//TypeCastToken::~TypeCastToken()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//SimpleToken::TokenType TypeCastToken::getTypeToCastTo() const
//{
//    return typeToCastTo;
//}

//QString TypeCastToken::printValue() const
//{
//    QString typeRealName;
//    switch(typeToCastTo)
//    {
//    case SimpleToken::Integer:
//        typeRealName = QString("(Integer)");
//        break;
//    case SimpleToken::Double:
//        typeRealName = QString("(Double)");
//        break;
//    case SimpleToken::Bool:
//        typeRealName = QString("(Bool)");
//        break;
//    case SimpleToken::String:
//        typeRealName = QString("(String)");
//        break;
//    default:
//        typeRealName = QString("(ERROR)");
//        break;
//    }

//    return QString("(%1)").arg(typeRealName);
//}

//QString TypeCastToken::printToken() const
//{
//    return QString("{TypeCastToken}:{%1}").arg(printValue());
//}


TypeNameToken::TypeNameToken(SimpleNode::ValueTypes type, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::TypeName, TokenPos, TokenLen),
    type(type)
{

}

TypeNameToken::~TypeNameToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::ValueTypes TypeNameToken::getType() const
{
    return type;
}

QString TypeNameToken::printValue() const
{
    return QString("(%1)").arg(SimpleNode::getHumanReadableTypeNameToValueType(type));
}

QString TypeNameToken::printToken() const
{
    return QString("{TypeNameToken}:{%1}").arg(printValue());
}

SemiColonDelimToken::SemiColonDelimToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::SemiColonDelim, TokenPos, TokenLen)
{

}

SemiColonDelimToken::~SemiColonDelimToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString SemiColonDelimToken::printValue() const
{
    return QString(";");
}

QString SemiColonDelimToken::printToken() const
{
    return QString("{SemiColonDelimToken}:{%1}").arg(printValue());
}

ReturnKeywordToken::ReturnKeywordToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::ReturnKeyword, TokenPos, TokenLen)
{

}

ReturnKeywordToken::~ReturnKeywordToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString ReturnKeywordToken::printValue() const
{
    return QString("return");
}

QString ReturnKeywordToken::printToken() const
{
    return QString("{ReturnKeywordToken}:{%1}").arg(printValue());
}

LCurlyParanToken::LCurlyParanToken(const int TokenPos, const int TokenLen) :
    OperationToken(SimpleToken::LCurlyParan, TokenPos, TokenLen)
{

}

LCurlyParanToken::~LCurlyParanToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString LCurlyParanToken::printValue() const
{
    return QString("{");
}

QString LCurlyParanToken::printToken() const
{
    return QString("{LCurlyParanToken}:{%1}").arg(printValue());
}

RCurlyParanToken::RCurlyParanToken(const int TokenPos, const int TokenLen) :
    OperationToken(SimpleToken::RCurlyParan, TokenPos, TokenLen)
{

}

RCurlyParanToken::~RCurlyParanToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString RCurlyParanToken::printValue() const
{
    return QString("}");
}

QString RCurlyParanToken::printToken() const
{
    return QString("{RCurlyParanToken}:{%1}").arg(printValue());
}

QMarkToken::QMarkToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::QMark, TokenPos, TokenLen)
{

}

QMarkToken::~QMarkToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString QMarkToken::printValue() const
{
    return QString("?");
}

QString QMarkToken::printToken() const
{
    return QString("{QMarkToken}:{%1}").arg(printValue());
}

ColonToken::ColonToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::Colon, TokenPos, TokenLen)
{

}

ColonToken::~ColonToken()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString ColonToken::printValue() const
{
    return QString(":");
}

QString ColonToken::printToken() const
{
    return QString("{ColonToken}:{%1}").arg(printValue());
}

