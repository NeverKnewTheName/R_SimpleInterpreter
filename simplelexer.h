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
 * 7 - Data
 *  -- 8 Index of data
 * 9 - Operation
 *  -- 10 Type
 *
 */

#ifndef SIMPLELEXER_H
#define SIMPLELEXER_H

#include <QObject>
#include <QVector>
#include <QString>
#include <QRegularExpression>
#include <QErrorMessage>

#include "simpletoken.h"

class SimpleLexer : public QObject
{
    Q_OBJECT
public:
    typedef QVector<SimpleToken*> TokenList;
    explicit SimpleLexer(QObject *parent = NULL);
    SimpleLexer(const QString &InputString, QObject *parent = NULL);
    ~SimpleLexer();
    void setStringForLexer(const QString &InputString);

    SharedSimpleTokenPtr getNextToken();
    void LexErrorAtToken(SharedSimpleTokenPtr ErrorAtToken, int type);

signals:
    void LexerPosAt(int pos, int len);
    void LexerErrorHTMLMsg(QString HTMLFormattedErrMsg);

private:
    /**
     * \brief Removes whitespace characters from the string which are not contained in a SubString
     * \param string
     */
    void RemoveWhitespacesFromString(QString &string);
    QRegularExpression regEx;
    QString InputString;
    QString LexerString;
    SharedSimpleTokenPtr CurrentToken;
    int PosInInputString;
};

#endif // SIMPLELEXER_H
