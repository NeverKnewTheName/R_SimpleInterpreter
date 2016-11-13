#include "r_simpleinterpreter.h"
#include "ui_r_simpleinterpreter.h"

#include "simpletoken.h"
#include "simplelexer.h"
#include <QDebug>

R_SimpleInterpreter::R_SimpleInterpreter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::R_SimpleInterpreter)
{
    ui->setupUi(this);
}

R_SimpleInterpreter::~R_SimpleInterpreter()
{
    delete ui;
}

void R_SimpleInterpreter::on_pushButton_clicked()
{
    QString TextToInterpret = ui->lineEdit->text();
    QVector<quint8> data;
    data.append(0);
    data.append(1);
    data.append(2);
    data.append(3);
    data.append(4);
    data.append(5);
    data.append(6);
    data.append(7);


//    AdditionToken *add = new AdditionToken();
//    SimpleToken *HiddenAdd = add;
//    BinaryOperationToken *BinaryAdd = add;

//    qDebug() << "Value1 type:" << value1.getValueType();
//    qDebug() << "Value2 type:" << value2.getValueType();

//    qDebug() << "add - SimpleToken -> TokenType: "  << HiddenAdd->getTokenType();
//    qDebug() << "add - OperationToken -> ArityType: "  << static_cast<OperationToken*>(HiddenAdd)->getArityType();
//    qDebug() << "add - BinaryOperationToken -> OperationType: "  << static_cast<BinaryOperationToken*>(HiddenAdd)->getOpType();
//    qDebug() << "add - BinaryArithmeticOperationToken -> Operation: "  << static_cast<BinaryArithmeticOperationToken*>(HiddenAdd)->getOp();

//    qDebug() << value1.printToken() << " + " << value2.printToken();
//    qDebug() << BinaryAdd->DoOperation(value1,value2).printToken();

    SimpleLexer lex;
    QVector<SimpleToken*> Tokens = lex.parseString(TextToInterpret);
    int TokensSize = Tokens.size();
//    ValueToken Result(*(dynamic_cast<const ValueToken*>(Tokens.value(0))));
    for(int i = 0; i < TokensSize; i++)
    {
        const SimpleToken *token = Tokens.at(i);
        switch(token->getTokenType())
        {
        case SimpleToken::Value:
            qDebug() << "Value \t" << dynamic_cast<const ValueToken*>(token)->printToken();
            break;
        case SimpleToken::Data:
            qDebug() << "Data \t" << dynamic_cast<const DataToken*>(token)->printToken();
            break;
        case SimpleToken::Operation:
        {
            const OperationToken *opToken = dynamic_cast<const OperationToken*>(token);
            qDebug() << "Operation \t";// << opToken->printToken();
            switch(opToken->getArityType())
            {
            case OperationToken::Unary:
                qDebug() << "Unary \t" << dynamic_cast<const UnaryOperationToken*>(opToken)->printToken();
                break;
            case OperationToken::Binary:
            {
                qDebug() << "Binary \t" << dynamic_cast<const BinaryOperationToken*>(opToken)->printToken();
    //                const SimpleToken *FirstOperand = Tokens.value(i-1);
    //                if(FirstOperand->getTokenType() == SimpleToken::Data)
    //                {
    //                    FirstOperand = new ValueToken(dynamic_cast<const DataToken*>(FirstOperand)->getData(data));
    //                }
//                const SimpleToken *SecondOperand = Tokens.value(i+1);
//                if(SecondOperand->getTokenType() == SimpleToken::Data)
//                {
//                    SecondOperand = new ValueToken(dynamic_cast<const DataToken*>(SecondOperand)->getData(data));
//                }
//                Result = dynamic_cast<const BinaryOperationToken*>(opToken)->DoOperation(Result,*(dynamic_cast<const ValueToken*>(SecondOperand)));
//                qDebug() << "Result \t" << Result.printToken();
            }
                break;
            case OperationToken::Ternary:
                qDebug() << "Ternary \t" << dynamic_cast<const TernaryOperationToken*>(opToken)->printToken();
                break;
            }
        }
            break;
        default:
            qDebug() << "ERROR" << " in Token";
        }
    }
//    ui->lineEdit_2->setText(Result.printValue());
}
