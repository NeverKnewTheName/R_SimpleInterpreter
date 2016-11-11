#include "simplelexer.h"
#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include <QDebug>

SimpleLexer::SimpleLexer()
{

}

QVector<SimpleToken*> SimpleLexer::parseString(QString stringToParse)
{
    QVector<SimpleToken*> Tokens;
    QRegularExpression regEx(
                QString("((\".*\")|((\\d+(\\.\\d+)?)|(true|false))|(D(\\d+))|([<>!=]?=|[<>]{1,2}|[&\\|\\^\\+\\-]{1,2}|[!~\\*\\/%]))")
                );
    RemoveWhitespacesFromString(stringToParse);
    qDebug() << stringToParse;
    QRegularExpressionMatch regExMatch = regEx.match(stringToParse);
    while(regExMatch.hasMatch())
    {
        qDebug() << "ToEvaluate: " << regExMatch.captured();
        if(!regExMatch.captured(2).isNull())
        {
            //IsString
            qDebug() << "IsString";
            qDebug() << regExMatch.captured(2);
            Tokens.append(new ValueToken(regExMatch.captured(2)));
        }
        else if(!regExMatch.captured(3).isNull())
        {
            //IsValue
            qDebug() << "IsValue";
            if(!regExMatch.captured(4).isNull()) //Number
            {
                //IsNumber
                qDebug() << "IsNumber";
                if(!regExMatch.captured(5).isNull())
                {
                    //IsDouble
                    qDebug() << "IsDouble";
                    qDebug() << regExMatch.captured(3);
                    Tokens.append(new ValueToken(regExMatch.captured(3).toDouble()));
                }
                else
                {
                    //IsInteger
                    qDebug() << "IsInteger";
                    qDebug() << regExMatch.captured(3);
                    Tokens.append(new ValueToken(regExMatch.captured(3).toInt()));
                }
            }
            else if(!regExMatch.captured(6).isNull())
            {
                //Boolean
                qDebug() << "Boolean";
                qDebug() << regExMatch.captured(3);
                Tokens.append(new ValueToken(regExMatch.captured(3).compare(QString("true")) ? false : true));
            }
        }
        else if(!regExMatch.captured(7).isNull())
        {
            //IsData
            qDebug() << "IsData";
            qDebug() << regExMatch.captured(7);
            int dataIndex = regExMatch.captured(8).toInt(); //DataIndex
            Tokens.append(new DataToken(dataIndex));
        }
        else if(!regExMatch.captured(9).isNull())
        {
            //IsOperator
            qDebug() << "IsOperator";
            qDebug() << regExMatch.captured(9);
            QString operatorString = regExMatch.captured(9);
            if(!operatorString.compare(QString("+")))
            {
                //Addition
                Tokens.append(new AdditionToken());
            }
            else if(!operatorString.compare(QString("-")))
            {
                //Subtraction
                Tokens.append(new SubtractionToken());
            }
            else if(!operatorString.compare(QString("*")))
            {
                //Multiplication
                Tokens.append(new MultiplicationToken());
            }
            else if(!operatorString.compare(QString("/")))
            {
                //Division
                Tokens.append(new DivisionToken());
            }
            else if(!operatorString.compare(QString("%")))
            {
                //Modulo
                Tokens.append(new ModuloToken());
            }
        }
        stringToParse.replace(regExMatch.capturedStart(),regExMatch.capturedLength(),QString(""));
        regExMatch = regEx.match(stringToParse);
        qDebug() << "";
    }

    return Tokens;
}

/**
 * \brief Removes whitespace characters from the string which are not contained in a SubString
 * \param string
 */
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
