#ifndef SIMPLELEXER_H
#define SIMPLELEXER_H

#include <QVector>
#include <QString>

#include "simpletoken.h"

class SimpleLexer
{
public:
    SimpleLexer();

    QVector<SimpleToken*> parseString(QString stringToParse);

private:
    void RemoveWhitespacesFromString(QString &string);
};

#endif // SIMPLELEXER_H
