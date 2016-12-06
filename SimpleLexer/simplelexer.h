/**
 *
 * Capture Groups:
 *
 * 1 - Whole Result
 * 2 - TypeName
 * 3 - String
 * 4 - Value
 *  -- 5 Number
 *  -- 6 Part after point -> Detects Double Values!
 *  -- 7 Bool Detects if a boolean value was found
 * 8 - Data
 *  -- 9 Index of data
 * 10 - Operation
 * 11 Identifier
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

    void ResetLexerToToken(SharedSimpleTokenPtr TokenToResetTo);

    SharedSimpleTokenPtr peekAtNextToken();
    SharedSimpleTokenPtr getNextToken(bool consume = true);
    void LexErrorAtToken(SharedSimpleTokenPtr ErrorAtToken, int type, QString details = QString());

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

    static const QString LexerRegularExpression;
};

#endif // SIMPLELEXER_H
