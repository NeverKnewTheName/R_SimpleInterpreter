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

#include <QStandardItemModel>
#include <QStandardItem>

R_SimpleInterpreter::R_SimpleInterpreter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::R_SimpleInterpreter),
    GlobalSymbolTable(QString("GlobalSymblTbl"))
{
    ui->setupUi(this);
    ui->textEdit->hide();
    GlobalSymbolTable.addEntry(QString("D0"), new VariableSymbol(QString("D0"), SimpleNode::Integer, new ValueNode(10)));
    GlobalSymbolTable.addEntry(QString("D1"), new VariableSymbol(QString("D1"), SimpleNode::Integer, new ValueNode(11)));
    GlobalSymbolTable.addEntry(QString("D2"), new VariableSymbol(QString("D2"), SimpleNode::Integer, new ValueNode(12)));
    GlobalSymbolTable.addEntry(QString("D3"), new VariableSymbol(QString("D3"), SimpleNode::Integer, new ValueNode(13)));
    GlobalSymbolTable.addEntry(QString("D4"), new VariableSymbol(QString("D4"), SimpleNode::Integer, new ValueNode(14)));
    GlobalSymbolTable.addEntry(QString("D5"), new VariableSymbol(QString("D5"), SimpleNode::Integer, new ValueNode(15)));
    GlobalSymbolTable.addEntry(QString("D6"), new VariableSymbol(QString("D6"), SimpleNode::Integer, new ValueNode(16)));
    GlobalSymbolTable.addEntry(QString("D7"), new VariableSymbol(QString("D7"), SimpleNode::Integer, new ValueNode(17)));
    GlobalSymbolTable.addEntry(QString("xxVARxx"), new VariableSymbol(QString("xxVARxx"), SimpleNode::Integer, new ValueNode(1)));


    lex = new SimpleLexer(this->parent());
    interpr = NULL;
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

    SimpleParser parse(lex, GlobalSymbolTable);
    SimpleInterpreter interpreter(&parse);

    ValueNode *result = interpreter.interpret();
    qDebug() << "Result Node: " << result->printNode();
    qDebug() << "Result Value: " << result->getValue();

    disconnect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);
    disconnect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);

    if(result->getReturnType() == SimpleNode::ErrorType)
        return;

    QStandardItemModel *SymbolTableModel = new QStandardItemModel(this);
    populateSymbolTableView(&GlobalSymbolTable, SymbolTableModel->invisibleRootItem());
    ui->SymbolTableView->setModel(SymbolTableModel);

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

void R_SimpleInterpreter::populateSymbolTableView(SymbolTable *symbolTable, QStandardItem *SymbolTableModel)
{
    for(SymbolTableEntry *entry : symbolTable->getSymbolTableAsSequence())
    {
        QStandardItem *entryItem = new QStandardItem(entry->PrintSymbolType());
        QList<QStandardItem*> row;
        row.append(entryItem);
        row.append(new QStandardItem(entry->PrintToSymbolToString()));
        SymbolTableModel->appendRow(row);
        if(entry->getType() == SymbolTableEntry::SubSymbolTable)
        {
            populateSymbolTableView(dynamic_cast<SymbolTable*>(entry), entryItem);
        }
    }
}
