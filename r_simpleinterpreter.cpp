#include "r_simpleinterpreter.h"
#include "ui_r_simpleinterpreter.h"

#include "simpletoken.h"
#include "simplelexer.h"
#include "simpleparser.h"
#include "simplesymboltable.h"
#include "simpleinterpreter.h"

#include <QMessageBox>
#include <QThread>
#include <QDebug>

R_SimpleInterpreter::R_SimpleInterpreter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::R_SimpleInterpreter),
    lex(new SimpleLexer(parent))
{
    ui->setupUi(this);
    ui->textEdit->hide();
}

R_SimpleInterpreter::~R_SimpleInterpreter()
{
    delete ui;
}

void R_SimpleInterpreter::on_pushButton_clicked()
{
    ui->textEdit->hide();
    QString TextToInterpret = ui->plainTextEdit->document()->toPlainText();

    lex->setStringForLexer(TextToInterpret);

    connect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);
    connect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);

    SimpleParser parser(lex);

    SymbolTable &symblTbl = parser.getSymblTbl();
    symblTbl.addEntry(QString("D0"), new VariableSymbol(SimpleNode::Integer, new ValueNode(10)));
    symblTbl.addEntry(QString("D1"), new VariableSymbol(SimpleNode::Integer, new ValueNode(11)));
    symblTbl.addEntry(QString("D2"), new VariableSymbol(SimpleNode::Integer, new ValueNode(12)));
    symblTbl.addEntry(QString("D3"), new VariableSymbol(SimpleNode::Integer, new ValueNode(13)));
    symblTbl.addEntry(QString("D4"), new VariableSymbol(SimpleNode::Integer, new ValueNode(14)));
    symblTbl.addEntry(QString("D5"), new VariableSymbol(SimpleNode::Integer, new ValueNode(15)));
    symblTbl.addEntry(QString("D6"), new VariableSymbol(SimpleNode::Integer, new ValueNode(16)));
    symblTbl.addEntry(QString("D7"), new VariableSymbol(SimpleNode::Integer, new ValueNode(17)));
    symblTbl.addEntry(QString("xxVARxx"), new VariableSymbol(SimpleNode::Integer, new ValueNode(1)));

    SimpleInterpreter interpreter(&parser);
    ValueNode *result = interpreter.interpret();
    qDebug() << "Result Node: " << result->printNode();
    qDebug() << "Result Value: " << result->getValue();

    disconnect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);
    disconnect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);

    ui->lineEdit_2->setText(result->getValue().value<QString>());
}

void R_SimpleInterpreter::lexerPosChanged(int pos, int len)
{
    QString TextToInterpret = ui->plainTextEdit->document()->toPlainText();

    while((TextToInterpret.length() > pos) && (TextToInterpret.at(pos).toLatin1() == ' '))
    {
        pos++;
    }

    TextToInterpret.insert(pos,QChar('#'));
    if(len != 0)
    {
        TextToInterpret.insert(pos+1+len,QChar('#'));
    }

    ui->plainTextEdit->setPlainText(TextToInterpret);
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.setPosition(pos);
    cursor.setPosition(pos+len+2,QTextCursor::KeepAnchor);
    ui->plainTextEdit->setTextCursor(cursor);
    ui->plainTextEdit->setFocus();
}

void R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg(QString HTMLFormattedErrMsg)
{
    ui->textEdit->setText(HTMLFormattedErrMsg);
    ui->textEdit->show();
//    QMessageBox::critical(this,
//                          QString("Parser ERROR"),
//                          HTMLFormattedErrMsg,
//                          QMessageBox::Ok);
}
