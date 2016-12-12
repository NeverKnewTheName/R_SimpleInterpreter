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
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
        typeString = QString("(");
        break;
    case LCurlyParan:
        typeString = QString("{");
        break;
    case RParan:
        typeString = QString(")");
        break;
    case RCurlyParan:
        typeString = QString("}");
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
        typeString = QString("++");
        break;
    case Decrement:
        typeString = QString("--");
        break;
    case Plus:
        typeString = QString("+");
        break;
    case Minus:
        typeString = QString("-");
        break;
    case LogicalNegation:
        typeString = QString("!");
        break;
    case OnesComplement:
        typeString = QString("~");
        break;
    case Multiplication:
        typeString = QString("*");
        break;
    case Division:
        typeString = QString("/");
        break;
    case Modulo:
        typeString = QString("%");
        break;
    case LogicalAND:
        typeString = QString("&&");
        break;
    case LogicalOR:
        typeString = QString("||");
        break;
    case LogicalXOR:
        typeString = QString("^^");
        break;
    case Greater:
        typeString = QString(">");
        break;
    case Lower:
        typeString = QString("<");
        break;
    case Equal:
        typeString = QString("==");
        break;
    case EqualOrGreater:
        typeString = QString(">=");
        break;
    case EqualOrLower:
        typeString = QString("<=");
        break;
    case Unequal:
        typeString = QString("!=");
        break;
    case BitwiseAND:
        typeString = QString("&");
        break;
    case BitwiseOR:
        typeString = QString("|");
        break;
    case BitwiseXOR:
        typeString = QString("^");
        break;
    case LeftShift:
        typeString = QString("<<");
        break;
    case RightShift:
        typeString = QString(">>");
        break;
    case Assign:
        typeString = QString("=");
        break;
    case QMark:
        typeString = QString("?");
        break;
    case Colon:
        typeString = QString(":");
        break;
    case CommaDelim:
        typeString = QString(",");
        break;
    case SemiColonDelim:
        typeString = QString(";");
        break;
    case If:
        typeString = QString("if");
        break;
    case Else:
        typeString = QString("else");
        break;
    case Switch:
        typeString = QString("switch");
        break;
    case Case:
        typeString = QString("case");
        break;
    case DefaultLabel:
        typeString = QString("default");
        break;
    case For:
        typeString = QString("for");
        break;
    case While:
        typeString = QString("while");
        break;
    case Do:
        typeString = QString("do");
        break;
    case Break:
        typeString = QString("break");
        break;
    case Continue:
        typeString = QString("continue");
        break;
    case ReturnKeyword:
        typeString = QString("return");
        break;
    case EOFToken:
        typeString = QString("EOF");
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
//    #ifdef __DEBUG_OUTPUT__
//    qDebug() << __PRETTY_FUNCTION__;
//#endif
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
//    #ifdef __DEBUG_OUTPUT__
//    qDebug() << __PRETTY_FUNCTION__;
//#endif
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
//    #ifdef __DEBUG_OUTPUT__
//    qDebug() << __PRETTY_FUNCTION__;
//#endif
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
//    #ifdef __DEBUG_OUTPUT__
//    qDebug() << __PRETTY_FUNCTION__;
//#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
//    #ifdef __DEBUG_OUTPUT__
//    qDebug() << __PRETTY_FUNCTION__;
//#endif
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


TypeNameToken::TypeNameToken(Node::ValueTypes type, const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::TypeName, TokenPos, TokenLen),
    type(type)
{

}

TypeNameToken::~TypeNameToken()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

Node::ValueTypes TypeNameToken::getType() const
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
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
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QString ColonToken::printValue() const
{
    return QString(":");
}

QString ColonToken::printToken() const
{
    return QString("{ColonToken}:{%1}").arg(printValue());
}


ControlToken::ControlToken(SimpleToken::TokenType type, const int TokenPos, const int TokenLen) :
    SimpleToken(type,TokenPos,TokenLen)
{

}

ControlToken::~ControlToken()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QString ControlToken::printValue() const
{
    return SimpleToken::convertTokenTypeToString(type);
}

QString ControlToken::printToken() const
{
    return QString("{ControlToken}:{%1}").arg(printValue());
}

CommaDelimToken::CommaDelimToken(const int TokenPos, const int TokenLen) :
    SimpleToken(SimpleToken::CommaDelim, TokenPos, TokenLen)
{

}

CommaDelimToken::~CommaDelimToken()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

QString CommaDelimToken::printValue() const
{
    return SimpleToken::convertTokenTypeToString(type);
}

QString CommaDelimToken::printToken() const
{
    return QString("{CommaDelimToken}:{%1}").arg(printValue());
}
