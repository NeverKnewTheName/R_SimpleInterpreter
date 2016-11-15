#include "r_simpleinterpreter.h"
#include "ui_r_simpleinterpreter.h"

#include "simpletoken.h"
#include "simplelexer.h"
#include "simpleparser.h"
#include "simplesymboltable.h"
#include "simpleinterpreter.h"
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

    SimpleLexer lex(TextToInterpret);
    SimpleParser parser(&lex);

    SymbolTable &symblTbl = parser.getSymblTbl();
    symblTbl.addEntry(QString("D0"), 10);
    symblTbl.addEntry(QString("D1"), 11);
    symblTbl.addEntry(QString("D2"), 12);
    symblTbl.addEntry(QString("D3"), 13);
    symblTbl.addEntry(QString("D4"), 14);
    symblTbl.addEntry(QString("D5"), 15);
    symblTbl.addEntry(QString("D6"), 16);
    symblTbl.addEntry(QString("D7"), 17);

//    SimpleNode *tree = parser.parse();
    SimpleInterpreter interpreter(&parser);
    ValueNode *result = interpreter.interpret();
    qDebug() << result->printNode();
    qDebug() << result->getValue();

    ui->lineEdit_2->setText(result->getValue().value<QString>());
}
