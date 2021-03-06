#include "simplelexer.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QMessageBox>
#include <QDebug>
#include <QThread>

#define REGEX_WHOLE                 1
#define REGEX_RETRNKW               2
#define REGEX_TYPENAME              3
#define REGEX_STRING                4
#define REGEX_VALUE                 5
#define REGEX_NUMBER                6
#define REGEX_NUMBER_AFTER_POINT    7
#define REGEX_VALUE_BOOL            8
#define REGEX_DATA                  9
#define REGEX_DATA_INDEX            10
#define REGEX_OPERATION             11
#define REGEX_IDENTIFIER            12

const QString SimpleLexer::LexerRegularExpression("((if|else|switch|case|default|for|do|while|continue|break|return)|(Integer|Double|Bool|String|Void)|(?:\"((?:\\\\\"|.)*?)\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|(\\(|\\{|\\)|\\}|[<>!=]?=|<{1,2}|>{1,2}|&{1,2}|\\|{1,2}|\\^{1,2}|\\+{1,2}|\\-{1,2}|[!~\\*\\/%\\?;,:])|([_a-zA-Z]\\w*))");

SimpleLexer::SimpleLexer(QObject *parent) :
    QObject(parent),
    regEx(LexerRegularExpression),
    CurrentToken(new EOFToken(0,0)),
    PosInInputString(0)
{
}

SimpleLexer::SimpleLexer(const QString &InputString, QObject *parent) :
    QObject(parent),
    regEx(LexerRegularExpression),
    InputString(InputString),
    LexerString(InputString),
    CurrentToken(new EOFToken(0,0)),
    PosInInputString(0)
{
}

SimpleLexer::~SimpleLexer()
{
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

void SimpleLexer::setStringForLexer(const QString &InputString)
{
    this->InputString = InputString;
    this->LexerString = InputString;
    this->CurrentToken = SharedSimpleTokenPtr(new EOFToken(0,0));
    this->PosInInputString = 0;
}

void SimpleLexer::ResetLexerToToken(SharedSimpleTokenPtr TokenToResetTo)
{
    PosInInputString = TokenToResetTo->getTokenPos() + TokenToResetTo->getTokenLen();
    CurrentToken = TokenToResetTo;
}

SharedSimpleTokenPtr SimpleLexer::peekAtNextToken()
{
#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
    return getNextToken(false);
}

SharedSimpleTokenPtr SimpleLexer::getNextToken(bool consume)
{
    SharedSimpleTokenPtr Token;
//    for(int i = 0;;i++)
//    {
//        if(LexerString.at(PosInInputString+i).toLatin1() != ' ')
//            break;
//        PosInInputString++;
//    }
    QRegularExpressionMatch regExMatch = regEx.match(LexerString,PosInInputString);

    if(regExMatch.hasMatch())
    {
#ifdef __DEBUG_OUTPUT__
        qDebug() << PosInInputString;
        qDebug() << regExMatch.captured(1);
        qDebug() << regExMatch.capturedStart(1);
        qDebug() << regExMatch.capturedLength(1);
#endif
        if(!regExMatch.captured(REGEX_RETRNKW).isNull())
        {
            //if|else|switch|case|for|do|while|continue|break|return
            QString keyword = regExMatch.captured(REGEX_RETRNKW);
            if(!keyword.compare(QString("if")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::If, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("else")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Else, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("switch")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Switch, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("case")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Case, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("default")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::DefaultLabel, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("for")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::For, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("do")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Do, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("while")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::While, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("continue")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Continue, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("break")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::Break, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!keyword.compare(QString("return")))
            {
                Token = SharedSimpleTokenPtr(new ControlToken(SimpleToken::ReturnKeyword, PosInInputString,regExMatch.capturedLength(REGEX_WHOLE)));
            }
        }
        else if(!regExMatch.captured(REGEX_TYPENAME).isNull())
        {
            //TypeName
            QString TypeName = regExMatch.captured(REGEX_TYPENAME);
            Node::ValueTypes type = Node::Integer;
            if(!TypeName.compare(QString("Integer")))
            {
                type = Node::Integer;
            }
            else if(!TypeName.compare(QString("Double")))
            {
                type = Node::Double;
            }
            else if(!TypeName.compare(QString("Bool")))
            {
                type = Node::Bool;
            }
            else if(!TypeName.compare(QString("String")))
            {
                type = Node::String;
            }
            else if(!TypeName.compare(QString("Void")))
            {
                type = Node::Void;
            }
            Token = SharedSimpleTokenPtr(new TypeNameToken(type, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
        }
        else if(!regExMatch.captured(REGEX_STRING).isNull())
        {
            //IsString
            QString string = regExMatch.captured(REGEX_STRING);
            Token = SharedSimpleTokenPtr(new ValueToken(QVariant(string), Node::String, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
        }
        else if(!regExMatch.captured(REGEX_VALUE).isNull())
        {
            //IsValue
            if(!regExMatch.captured(REGEX_NUMBER).isNull()) //Number
            {
                //IsNumber
                if(!regExMatch.captured(REGEX_NUMBER_AFTER_POINT).isNull())
                {
                    //IsDouble
                    Token = SharedSimpleTokenPtr(new ValueToken(QVariant(regExMatch.captured(REGEX_VALUE).toDouble()), Node::Double, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else
                {
                    //IsInteger
                    Token = SharedSimpleTokenPtr(new ValueToken(QVariant(regExMatch.captured(REGEX_VALUE).toInt()), Node::Integer, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
            }
            else if(!regExMatch.captured(REGEX_VALUE_BOOL).isNull())
            {
                //Boolean
                Token = SharedSimpleTokenPtr(new ValueToken(QVariant((regExMatch.captured(REGEX_VALUE).compare(QString("true")) ? false : true)), Node::Bool, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
            }
        }
        else if(!regExMatch.captured(REGEX_DATA).isNull())
        {
            //IsData
            int dataIndex = regExMatch.captured(REGEX_DATA_INDEX).toInt(); //DataIndex
            Token = SharedSimpleTokenPtr(new DataToken(dataIndex, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
        }
        else if(!regExMatch.captured(REGEX_OPERATION).isNull())
        {
            //IsOperator
            QString operatorString = regExMatch.captured(REGEX_OPERATION);
            if(!operatorString.compare(QString("(")))
            {
                //LPARAN
                Token = SharedSimpleTokenPtr(new LParanToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!operatorString.compare(QString(")")))
            {
                //RPARAN
                Token = SharedSimpleTokenPtr(new RParanToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!operatorString.compare(QString("{")))
            {
                //LPARAN
                Token = SharedSimpleTokenPtr(new LCurlyParanToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else if(!operatorString.compare(QString("}")))
            {
                //RPARAN
                Token = SharedSimpleTokenPtr(new RCurlyParanToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
            }
            else
                //Currently Increment and Decrement unsupported... Does it even make sense without variables??
                if(!operatorString.compare(QString("++")))
                {
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Increment, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("--")))
                {
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Decrement, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("+")))
                {
                    //Plus
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Plus, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("-")))
                {
                    //Minus
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Minus, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("!")))
                {
                    //LogicalNegation
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalNegation, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("~")))
                {
                    //OnesComplement
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::OnesComplement, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("*")))
                {
                    //Multiplication
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Multiplication, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("/")))
                {
                    //Division
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Division, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("%")))
                {
                    //Modulo
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Modulo, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("&&")))
                {
                    //LogicalAND
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalAND, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("||")))
                {
                    //LogicalOR
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalOR, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("^^")))
                {
                    //LogicalXOR
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalXOR, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(">")))
                {
                    //Greater
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Greater, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("<")))
                {
                    //Lower
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Lower, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("==")))
                {
                    //Equal
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Equal, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(">=")))
                {
                    //EqualOrGreater
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrGreater, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("<=")))
                {
                    //EqualOrLower
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrLower, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("!=")))
                {
                    //Unequal
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Unequal, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("&")))
                {
                    //AND
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseAND, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("|")))
                {
                    //OR
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseOR, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("^")))
                {
                    //XOR
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseXOR, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("<<")))
                {
                    //LeftShift
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LeftShift, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(">>")))
                {
                    //RightShift
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::RightShift, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("=")))
                {
                    //Assignment
                    Token = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Assign, PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString("?")))
                {
                    //QMark
                    Token = SharedSimpleTokenPtr(new QMarkToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(":")))
                {
                    //Colon
                    Token = SharedSimpleTokenPtr(new ColonToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(",")))
                {
                    //CommaDelim
                    Token = SharedSimpleTokenPtr(new CommaDelimToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
                else if(!operatorString.compare(QString(";")))
                {
                    //SemiColonDelim
                    Token = SharedSimpleTokenPtr(new SemiColonDelimToken(PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
                }
        }
        else if(!regExMatch.captured(REGEX_IDENTIFIER).isNull())
        {
            //Identifier -> Variable
            Token = SharedSimpleTokenPtr(new VariableIDToken(regExMatch.captured(REGEX_IDENTIFIER), PosInInputString, regExMatch.capturedLength(REGEX_WHOLE)));
        }
        if(consume)
        {
            PosInInputString = regExMatch.capturedStart() + regExMatch.capturedLength();
            while((LexerString.size() > PosInInputString) && (LexerString.at(PosInInputString).isSpace()))
            {
                PosInInputString++;
            }
        }
    }
    else
    {
        Token = SharedSimpleTokenPtr(new EOFToken(PosInInputString,0));
    }

    if(consume)
    {
        CurrentToken = Token;
    }

#ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__ << " new Token: " << Token->printToken();
#endif
    return Token;
}

void SimpleLexer::LexErrorAtToken(SharedSimpleTokenPtr ErrorAtToken, int type, QString details)
{
    QString FaultyInput = InputString;
    QString ErrorType = QString("Undefined");
    switch(type)
    {
    case 0:
        ErrorType = QString("SyntacticError");
        break;
    case 1:
        ErrorType = QString("TypeMismatch");
        break;
    case 2:
        ErrorType = QString("Unexpected EOF");
        break;
    case 3:
        ErrorType = QString("Expected EOF, but received a Token");
        break;
    case 4:
        ErrorType = QString("Symbol Error");
        break;
    default:
        ErrorType = QString("Unknown Error");
    }

    int TokenPos = ErrorAtToken->getTokenPos();
    int TokenLen = ErrorAtToken->getTokenLen();
    emit LexerPosAt(TokenPos, TokenLen);

    while((FaultyInput.length() > TokenPos) && (FaultyInput.at(TokenPos).toLatin1() == ' '))
    {
        TokenPos++;
    }

    FaultyInput.prepend(QString("<font size=\"4\">"));
    TokenPos += 15;
    FaultyInput.insert(TokenPos,QString("<font color=\"red\">#"));
    TokenPos += 19;
    if(TokenLen != 0)
    {
        TokenPos += TokenLen;
        FaultyInput.insert(TokenPos,QChar('#'));
        TokenPos++;
    }
    FaultyInput.insert(TokenPos,QString("</font>"));
    FaultyInput.append(QString("</font>"));
#ifdef __DEBUG_OUTPUT__
    qDebug() << FaultyInput;
#endif

    emit LexerErrorHTMLMsg(QString("<b>%1</b> at Token:<br><code><b>%2</b></code><br><br><code>%3</code>\n%4")
                           .arg(ErrorType)
                           .arg(ErrorAtToken->printToken())
                           .arg(FaultyInput)
                           .arg(details));
    //    QMessageBox::critical(&parent,
    //                          QString("Parser ERROR"),
    //                          QString("<b>%1</b> at Token:<br><code><b>%2</b></code><br><br><code>%3</code>")
    //                          .arg(ErrorType)
    //                          .arg(ErrorAtToken->printToken())
    //                          .arg(FaultyInput),
    //                          QMessageBox::Ok);
}

void SimpleLexer::RemoveWhitespacesFromString(QString &string)
{
    QRegularExpression regEx(
                QString("(\\s+)|(?:\".*\")"));
    QRegularExpressionMatch regExMatch = regEx.match(string);
    while(regExMatch.hasMatch())
    {
        if(!regExMatch.captured(1).isNull())
        {
            string.replace(regExMatch.capturedStart(1),regExMatch.capturedLength(1),QString(""));
            regExMatch = regEx.match(string,regExMatch.capturedStart(1));
        }
        else
        {
            regExMatch = regEx.match(string, regExMatch.capturedEnd());
        }
    }
}
