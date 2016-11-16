#include "simplelexer.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QMessageBox>
#include <QDebug>
#include <QThread>


SimpleLexer::SimpleLexer(QObject *parent) :
    QObject(parent),
    regEx(QString("((?:\"(.*?)\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|(\\((Integer|Double|Bool|String)\\)|\\(|\\)|[<>!=]?=|<{1,2}|>{1,2}|&{1,2}|\\|{1,2}|\\^{1,2}|[\\+\\-!~\\*\\/%]))")),
    CurrentToken(new EOFToken(0,0)),
    PosInInputString(0)
{
}

SimpleLexer::SimpleLexer(const QString &InputString, QObject *parent) :
    QObject(parent),
    regEx(QString("((?:\"(.*?)\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|(\\((Integer|Double|Bool|String)\\)|\\(|\\)|[<>!=]?=|<{1,2}|>{1,2}|&{1,2}|\\|{1,2}|\\^{1,2}|[\\+\\-!~\\*\\/%]))")),
    InputString(InputString),
    LexerString(InputString),
    CurrentToken(new EOFToken(0,0)),
    PosInInputString(0)
{
}

SimpleLexer::~SimpleLexer()
{
}

void SimpleLexer::setStringForLexer(const QString &InputString)
{
    this->InputString = InputString;
    this->LexerString = InputString;
    this->CurrentToken = SharedSimpleTokenPtr(new EOFToken(0,0));
    this->PosInInputString = 0;
}

SharedSimpleTokenPtr SimpleLexer::getNextToken()
{
    QRegularExpressionMatch regExMatch = regEx.match(LexerString);

    if(regExMatch.hasMatch())
    {
        if(!regExMatch.captured(2).isNull())
        {
            //IsString
            QString string = regExMatch.captured(2);
            CurrentToken = SharedSimpleTokenPtr(new StringToken(string, PosInInputString, regExMatch.capturedLength(1)));
        }
        else if(!regExMatch.captured(3).isNull())
        {
            //IsValue
            if(!regExMatch.captured(4).isNull()) //Number
            {
                //IsNumber
                if(!regExMatch.captured(5).isNull())
                {
                    //IsDouble
                    CurrentToken = SharedSimpleTokenPtr(new DoubleToken(regExMatch.captured(3).toDouble(), PosInInputString, regExMatch.capturedLength(1)));
                }
                else
                {
                    //IsInteger
                    CurrentToken = SharedSimpleTokenPtr(new IntegerToken(regExMatch.captured(3).toInt(), PosInInputString, regExMatch.capturedLength(1)));
                }
            }
            else if(!regExMatch.captured(6).isNull())
            {
                //Boolean
                CurrentToken = SharedSimpleTokenPtr(new BoolToken((regExMatch.captured(3).compare(QString("true")) ? false : true), PosInInputString, regExMatch.capturedLength(1)));
            }
        }
        else if(!regExMatch.captured(7).isNull())
        {
            //IsData
            int dataIndex = regExMatch.captured(8).toInt(); //DataIndex
            CurrentToken = SharedSimpleTokenPtr(new DataToken(dataIndex, PosInInputString, regExMatch.capturedLength(1)));
        }
        else if(!regExMatch.captured(9).isNull())
        {
            //IsOperator
            QString operatorString = regExMatch.captured(9);
            QString typeString = regExMatch.captured(10);
            if(!typeString.isNull())
            {
                //TypeCast
                SimpleToken::TokenType typeToCastTo = SimpleToken::Integer; // IF ERROR -> CAST TO INTEGER
                if(!typeString.compare("Integer"))
                {
                    typeToCastTo = SimpleToken::Integer;
                }
                else if(!typeString.compare("Double"))
                {
                    typeToCastTo = SimpleToken::Double;
                }
                else if(!typeString.compare("Bool"))
                {
                    typeToCastTo = SimpleToken::Bool;
                }
                else if(!typeString.compare("String"))
                {
                    typeToCastTo = SimpleToken::String;
                }
                CurrentToken = SharedSimpleTokenPtr(new TypeCastToken(typeToCastTo, PosInInputString, regExMatch.capturedLength(1)));
            }
            else if(!operatorString.compare(QString("(")))
            {
                //Plus
                CurrentToken = SharedSimpleTokenPtr(new LParanToken(PosInInputString, regExMatch.capturedLength(1)));
            }
            else if(!operatorString.compare(QString(")")))
            {
                //Minus
                CurrentToken = SharedSimpleTokenPtr(new RParanToken(PosInInputString, regExMatch.capturedLength(1)));
            }
            else
                //Currently Increment and Decrement unsupported... Does it even make sense without variables??
                //            if(!operatorString.compare(QString("++")))
                //            {
                //                CurrentToken = SharedSimpleTokenPtr(OperationToken(SimpleToken::Increment));
                //            }
                //            else if(!operatorString.compare(QString("--")))
                //            {
                //                CurrentToken = SharedSimpleTokenPtr(OperationToken(SimpleToken::Decrement));
                //            }
                //            else
                if(!operatorString.compare(QString("+")))
                {
                    //Plus
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Plus, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("-")))
                {
                    //Minus
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Minus, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("!")))
                {
                    //LogicalNegation
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalNegation, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("~")))
                {
                    //OnesComplement
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::OnesComplement, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("*")))
                {
                    //Multiplication
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Multiplication, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("/")))
                {
                    //Division
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Division, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("%")))
                {
                    //Modulo
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Modulo, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("&&")))
                {
                    //LogicalAND
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalAND, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("||")))
                {
                    //LogicalOR
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalOR, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("^^")))
                {
                    //LogicalXOR
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalXOR, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString(">")))
                {
                    //Greater
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Greater, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("<")))
                {
                    //Lower
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Lower, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("==")))
                {
                    //Equal
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Equal, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString(">=")))
                {
                    //EqualOrGreater
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrGreater, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("<=")))
                {
                    //EqualOrLower
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrLower, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("!=")))
                {
                    //Unequal
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Unequal, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("&")))
                {
                    //AND
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseAND, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("|")))
                {
                    //OR
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseOR, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("^")))
                {
                    //XOR
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseXOR, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("<<")))
                {
                    //LeftShift
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LeftShift, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString(">>")))
                {
                    //RightShift
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::RightShift, PosInInputString, regExMatch.capturedLength(1)));
                }
                else if(!operatorString.compare(QString("?:")))
                {
                    //Conditional
                    CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Conditional, PosInInputString, regExMatch.capturedLength(1)));
                }
        }
        PosInInputString += regExMatch.capturedStart() + regExMatch.capturedLength();
        LexerString.replace(regExMatch.capturedStart(),regExMatch.capturedLength(),QString(""));
        regExMatch = regEx.match(LexerString);
    }
    else
    {
        CurrentToken = SharedSimpleTokenPtr(new EOFToken(PosInInputString,0));
    }

    qDebug() << __PRETTY_FUNCTION__ << " new Token: " << CurrentToken->printToken();
    return CurrentToken;
}

void SimpleLexer::LexErrorAtToken(SharedSimpleTokenPtr ErrorAtToken, int type)
{
    QString FaultyInput = InputString;
    QString ErrorType = QString("Undefined");
    if(type == 0)
    {
        ErrorType = QString("SyntacticError");
    }
    else if (type == 1)
    {
        ErrorType = QString("TypeMismatch");
    }
    else if (type == 2)
    {
        ErrorType = QString("Unexpected EOF");
    }
    else if (type == 3)
    {
        ErrorType = QString("Expected EOF, but received a Token");
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
    qDebug() << FaultyInput;

    emit LexerErrorHTMLMsg(QString("<b>%1</b> at Token:<br><code><b>%2</b></code><br><br><code>%3</code>")
                           .arg(ErrorType)
                           .arg(ErrorAtToken->printToken())
                           .arg(FaultyInput));
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
