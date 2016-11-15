#include "simplelexer.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QErrorMessage>
#include <QDebug>

SimpleLexer::SimpleLexer(const QString &InputString) :
    regEx(QString("((?:\"(.*?)\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|(\\(|\\)|[<>!=]?=|<{1,2}|>{1,2}|&{1,2}|\\|{1,2}|\\^{1,2}|[\\+\\-!~\\*\\/%]))")),
    InputString(InputString),
    LexerString(InputString),
    CurrentToken(new EOFToken())
{
}

//SimpleLexer::TokenList SimpleLexer::parseString()
//{
//    TokenList Tokens;
//    /*
//     * Capture Groups:
//     * 1 - Whole Result
//     * 2 - String
//     * 3 - Value
//     *  -- 4 Number
//     *  -- 5 Part after point -> Detects Double Values!
//     *  -- 6 Bool Detects if a boolean value was found
//     * 6 - Data
//     *  -- 7 Index of data
//     * 7 - Operation
//     */
//    QRegularExpression regEx(
//                QString("((\".*\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|([<>!=]?=|<{1,2}|>{1,2}|&{1,2}|\\|{1,2}|\\^{1,2}|[\\+\\-!~\\*\\/%]))")
//                );
////    RemoveWhitespacesFromString(stringToParse);
//    qDebug() << stringToParse;
//    QRegularExpressionMatch regExMatch = regEx.match(stringToParse);
//    while(regExMatch.hasMatch())
//    {
//        qDebug() << "ToEvaluate: " << regExMatch.captured();
//        if(!regExMatch.captured(2).isNull())
//        {
//            //IsString
//            qDebug() << "IsString";
//            qDebug() << regExMatch.captured(2);
//            Tokens.append(new ValueToken(regExMatch.captured(2)));
//        }
//        else if(!regExMatch.captured(3).isNull())
//        {
//            //IsValue
//            qDebug() << "IsValue";
//            if(!regExMatch.captured(4).isNull()) //Number
//            {
//                //IsNumber
//                qDebug() << "IsNumber";
//                if(!regExMatch.captured(5).isNull())
//                {
//                    //IsDouble
//                    qDebug() << "IsDouble";
//                    qDebug() << regExMatch.captured(3);
//                    Tokens.append(new ValueToken(regExMatch.captured(3).toDouble()));
//                }
//                else
//                {
//                    //IsInteger
//                    qDebug() << "IsInteger";
//                    qDebug() << regExMatch.captured(3);
//                    Tokens.append(new ValueToken(regExMatch.captured(3).toInt()));
//                }
//            }
//            else if(!regExMatch.captured(6).isNull())
//            {
//                //Boolean
//                qDebug() << "Boolean";
//                qDebug() << regExMatch.captured(3);
//                Tokens.append(new ValueToken(regExMatch.captured(3).compare(QString("true")) ? false : true));
//            }
//        }
//        else if(!regExMatch.captured(7).isNull())
//        {
//            //IsData
//            qDebug() << "IsData";
//            qDebug() << regExMatch.captured(7);
//            int dataIndex = regExMatch.captured(8).toInt(); //DataIndex
//            Tokens.append(new DataToken(dataIndex));
//        }
//        else if(!regExMatch.captured(9).isNull())
//        {
//            //IsOperator
//            qDebug() << "IsOperator";
//            qDebug() << regExMatch.captured(9);
//            QString operatorString = regExMatch.captured(9);
//            //Currently Increment and Decrement unsupported... Does it even make sense without variables??
////            if(!operatorString.compare(QString("++")))
////            {
////                //Increment
////                Tokens.append(new IncrementToken());
////            }
////            else if(!operatorString.compare(QString("--")))
////            {
////                //Decrement
////                Tokens.append(new DecrementToken());
////            }
////            else
//            if(!operatorString.compare(QString("+")))
//            {
//                if(!Tokens.isEmpty() && Tokens.last()->getTokenType() != SimpleToken::Value && Tokens.last()->getTokenType() != SimpleToken::Data)
//                {
//                    //Positive
//                    Tokens.append(new PositiveToken());
//                }
//                else
//                {
//                    //Addition
//                    Tokens.append(new AdditionToken());
//                }
//            }
//            else if(!operatorString.compare(QString("-")))
//            {
//                if(!Tokens.isEmpty() && Tokens.last()->getTokenType() != SimpleToken::Value && Tokens.last()->getTokenType() != SimpleToken::Data)
//                {
//                    //Negative
//                    Tokens.append(new NegativeToken());
//                }
//                else
//                {
//                    //Subtraction
//                    Tokens.append(new SubtractionToken());
//                }
//            }
//            else if(!operatorString.compare(QString("!")))
//            {
//                //LogicalNegation
//                Tokens.append(new LogicalNegationToken());
//            }
//            else if(!operatorString.compare(QString("~")))
//            {
//                //OnesComplement
//                Tokens.append(new OnesComplementToken());
//            }
//            else if(!operatorString.compare(QString("*")))
//            {
//                //Multiplication
//                Tokens.append(new MultiplicationToken());
//            }
//            else if(!operatorString.compare(QString("/")))
//            {
//                //Division
//                Tokens.append(new DivisionToken());
//            }
//            else if(!operatorString.compare(QString("%")))
//            {
//                //Modulo
//                Tokens.append(new ModuloToken());
//            }
//            else if(!operatorString.compare(QString("&&")))
//            {
//                //LogicalAND
//                Tokens.append(new LogicalANDToken());
//            }
//            else if(!operatorString.compare(QString("||")))
//            {
//                //LogicalOR
//                Tokens.append(new LogicalORToken());
//            }
//            else if(!operatorString.compare(QString("^^")))
//            {
//                //LogicalXOR
//                Tokens.append(new LogicalXORToken());
//            }
//            else if(!operatorString.compare(QString(">")))
//            {
//                //Greater
//                Tokens.append(new GreaterToken());
//            }
//            else if(!operatorString.compare(QString("<")))
//            {
//                //Lower
//                Tokens.append(new LowerToken());
//            }
//            else if(!operatorString.compare(QString("==")))
//            {
//                //Equal
//                Tokens.append(new EqualToken());
//            }
//            else if(!operatorString.compare(QString(">=")))
//            {
//                //EqualOrGreater
//                Tokens.append(new EqualOrGreaterToken());
//            }
//            else if(!operatorString.compare(QString("<=")))
//            {
//                //EqualOrLower
//                Tokens.append(new EqualOrLowerToken());
//            }
//            else if(!operatorString.compare(QString("!=")))
//            {
//                //Unequal
//                Tokens.append(new UnequalToken());
//            }
//            else if(!operatorString.compare(QString("&")))
//            {
//                //AND
//                Tokens.append(new ANDToken());
//            }
//            else if(!operatorString.compare(QString("|")))
//            {
//                //OR
//                Tokens.append(new ORToken());
//            }
//            else if(!operatorString.compare(QString("^")))
//            {
//                //XOR
//                Tokens.append(new XORToken());
//            }
//            else if(!operatorString.compare(QString("<<")))
//            {
//                //LeftShift
//                Tokens.append(new LeftShiftToken());
//            }
//            else if(!operatorString.compare(QString(">>")))
//            {
//                //RightShift
//                Tokens.append(new RightShiftToken());
//            }
//            else if(!operatorString.compare(QString("?:")))
//            {
//                //Conditional
//                Tokens.append(new ConditionalToken());
//            }
//        }
//        stringToParse.replace(regExMatch.capturedStart(),regExMatch.capturedLength(),QString(""));
//        regExMatch = regEx.match(stringToParse);
//        qDebug() << "";
//    }
//    for(auto &token : Tokens)
//        qDebug() << token->printToken();
//    return Tokens;
//}

SharedSimpleTokenPtr SimpleLexer::getNextToken()
{
    QRegularExpressionMatch regExMatch = regEx.match(LexerString);

    if(regExMatch.hasMatch())
    {
        if(!regExMatch.captured(2).isNull())
        {
            //IsString
            QString string = regExMatch.captured(2);
            CurrentToken = SharedSimpleTokenPtr(new StringToken(string));
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
                    CurrentToken = SharedSimpleTokenPtr(new DoubleToken(regExMatch.captured(3).toDouble()));
                }
                else
                {
                    //IsInteger
                    CurrentToken = SharedSimpleTokenPtr(new IntegerToken(regExMatch.captured(3).toInt()));
                }
            }
            else if(!regExMatch.captured(6).isNull())
            {
                //Boolean
                CurrentToken = SharedSimpleTokenPtr(new BoolToken(regExMatch.captured(3).compare(QString("true")) ? false : true));
            }
        }
        else if(!regExMatch.captured(7).isNull())
        {
            //IsData
            int dataIndex = regExMatch.captured(8).toInt(); //DataIndex
            CurrentToken = SharedSimpleTokenPtr(new DataToken(dataIndex));
        }
        else if(!regExMatch.captured(9).isNull())
        {
            //IsOperator
            QString operatorString = regExMatch.captured(9);
            if(!operatorString.compare(QString("(")))
            {
                //Plus
                CurrentToken = SharedSimpleTokenPtr(new LParanToken());
            }
            else if(!operatorString.compare(QString(")")))
            {
                //Minus
                CurrentToken = SharedSimpleTokenPtr(new RParanToken());
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
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Plus));
            }
            else if(!operatorString.compare(QString("-")))
            {
                //Minus
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Minus));
            }
            else if(!operatorString.compare(QString("!")))
            {
                //LogicalNegation
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalNegation));
            }
            else if(!operatorString.compare(QString("~")))
            {
                //OnesComplement
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::OnesComplement));
            }
            else if(!operatorString.compare(QString("*")))
            {
                //Multiplication
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Multiplication));
            }
            else if(!operatorString.compare(QString("/")))
            {
                //Division
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Division));
            }
            else if(!operatorString.compare(QString("%")))
            {
                //Modulo
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Modulo));
            }
            else if(!operatorString.compare(QString("&&")))
            {
                //LogicalAND
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalAND));
            }
            else if(!operatorString.compare(QString("||")))
            {
                //LogicalOR
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalOR));
            }
            else if(!operatorString.compare(QString("^^")))
            {
                //LogicalXOR
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LogicalXOR));
            }
            else if(!operatorString.compare(QString(">")))
            {
                //Greater
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Greater));
            }
            else if(!operatorString.compare(QString("<")))
            {
                //Lower
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Lower));
            }
            else if(!operatorString.compare(QString("==")))
            {
                //Equal
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Equal));
            }
            else if(!operatorString.compare(QString(">=")))
            {
                //EqualOrGreater
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrGreater));
            }
            else if(!operatorString.compare(QString("<=")))
            {
                //EqualOrLower
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::EqualOrLower));
            }
            else if(!operatorString.compare(QString("!=")))
            {
                //Unequal
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Unequal));
            }
            else if(!operatorString.compare(QString("&")))
            {
                //AND
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseAND));
            }
            else if(!operatorString.compare(QString("|")))
            {
                //OR
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseOR));
            }
            else if(!operatorString.compare(QString("^")))
            {
                //XOR
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::BitwiseXOR));
            }
            else if(!operatorString.compare(QString("<<")))
            {
                //LeftShift
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::LeftShift));
            }
            else if(!operatorString.compare(QString(">>")))
            {
                //RightShift
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::RightShift));
            }
            else if(!operatorString.compare(QString("?:")))
            {
                //Conditional
                CurrentToken = SharedSimpleTokenPtr(new OperationToken(SimpleToken::Conditional));
            }
//            else if(!operatorString.compare(QString("(")))
//            {
//                //Opening Paranthesis
//                CurrentToken = SharedSimpleTokenPtr(OperationToken(OperationToken::LParan));
//            }
//            else if(!operatorString.compare(QString(")")))
//            {
//                //Closing Paranthesis
//                CurrentToken = SharedSimpleTokenPtr(OperationToken(OperationToken::RParan));
//            }
        }
        LexerString.replace(regExMatch.capturedStart(),regExMatch.capturedLength(),QString(""));
        regExMatch = regEx.match(LexerString);
    }
    else
    {
        CurrentToken = SharedSimpleTokenPtr(new EOFToken());
    }

    qDebug() << __PRETTY_FUNCTION__ << " new Token: " << CurrentToken->printToken();
    return CurrentToken;
}

void SimpleLexer::LexErrorAtToken(SharedSimpleTokenPtr ErrorAtToken, int type)
{
    QErrorMessage *error = new QErrorMessage();
    QString ErrorType = QString("Undefined");
    if(type == 0)
    {
        ErrorType = QString("SyntacticError");
    }
    else if (type == 1)
    {
        ErrorType = QString("TypeMismatch");
    }

    error->showMessage(QString("ErrorParsing Token: %1 -> %2").arg(ErrorAtToken->printToken()).arg(ErrorType));
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
