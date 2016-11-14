/**
 *
 * Capture Groups:
 *
 * 1 - Whole Result
 * 2 - String
 * 3 - Value
 *  -- 4 Number
 *  -- 5 Part after point -> Detects Double Values!
 *  -- 6 Bool Detects if a boolean value was found
 * 6 - Data
 *  -- 7 Index of data
 * 7 - Operation
 *
 */

#ifndef SIMPLELEXER_H
#define SIMPLELEXER_H

#include <QVector>
#include <QString>

#include "simpletoken.h"

class SimpleLexer
{
public:
    typedef QVector<SimpleToken*> TokenList;
    SimpleLexer(const QString &InputString);

    TokenList parseString(QString stringToParse);
    SimpleToken getNextToken();

private:
    /**
     * \brief Removes whitespace characters from the string which are not contained in a SubString
     * \param string
     */
    void RemoveWhitespacesFromString(QString &string);
    QRegularExpression regEx;
    const QString InputString;
    QString LexerString;
};

#endif // SIMPLELEXER_H
