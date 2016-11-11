#include "r_simpleinterpreter.h"
#include <QApplication>

#include "simpletoken.h"
#include "simplelexer.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    //    QApplication a(argc, argv);

    ValueToken value1(2);
    ValueToken value2(6);

    AdditionToken *add = new AdditionToken();
    SimpleToken *HiddenAdd = add;
    BinaryOperationToken *BinaryAdd = add;

    qDebug() << "Value1 type:" << value1.getValueType();
    qDebug() << "Value2 type:" << value1.getValueType();

    qDebug() << "add - SimpleToken -> TokenType: "  << HiddenAdd->getTokenType();
    qDebug() << "add - OperationToken -> ArityType: "  << static_cast<OperationToken*>(HiddenAdd)->getArityType();
    qDebug() << "add - BinaryOperationToken -> OperationType: "  << static_cast<BinaryOperationToken*>(HiddenAdd)->getOpType();
    qDebug() << "add - BinaryArithmeticOperationToken -> Operation: "  << static_cast<BinaryArithmeticOperationToken*>(HiddenAdd)->getOp();

    qDebug() << value1.printToken() << " + " << value2.printToken();
    qDebug() << BinaryAdd->DoOperation(value1,value2).printToken();

    SimpleLexer lex;
    QVector<SimpleToken*> Tokens = lex.parseString(QString("\"Result: \"    36 +4.9 *77.0 /8.01 -5 <<D0 "));
    for(SimpleToken *token : Tokens)
    {
        switch(token->getTokenType())
        {
        case SimpleToken::Value:
            qDebug() << static_cast<ValueToken*>(token)->printToken();
            break;
        case SimpleToken::Data:
            qDebug() << static_cast<DataToken*>(token)->printToken();
            break;
        case SimpleToken::Operation:
            qDebug() << "Operation";
            break;
        default:
            qDebug() << "ERROR" << " in Token";
        }
    }
    //    R_SimpleInterpreter w;
    //    w.show();

    return 0;
    //    return a.exec();
}
