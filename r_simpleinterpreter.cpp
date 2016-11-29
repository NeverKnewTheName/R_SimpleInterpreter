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
    VariableSymbolPtr newVarSymbol(new VariableSymbol(QString("D0"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(10));
    GlobalSymbolTable.addEntry(QString("D0"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D1"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(20));
    GlobalSymbolTable.addEntry(QString("D1"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D2"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(30));
    GlobalSymbolTable.addEntry(QString("D2"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D3"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(40));
    GlobalSymbolTable.addEntry(QString("D3"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D4"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(50));
    GlobalSymbolTable.addEntry(QString("D4"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D5"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(60));
    GlobalSymbolTable.addEntry(QString("D5"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D6"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(70));
    GlobalSymbolTable.addEntry(QString("D6"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("D7"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(80));
    GlobalSymbolTable.addEntry(QString("D7"), newVarSymbol);
    newVarSymbol = VariableSymbolPtr(new VariableSymbol(QString("xxVARxx"), SimpleNode::Integer));
    newVarSymbol->assignValue(ValueNode(1));
    GlobalSymbolTable.addEntry(QString("xxVARxx"), newVarSymbol);


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

//    lex->setStringForLexer(TextToInterpret);

//    connect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);
//    connect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);

//    SimpleParser parse(lex, GlobalSymbolTable);
//    SimpleInterpreter interpreter(&parse);

//    ValueNode *result = interpreter.interpret();
//    qDebug() << "Result Node: " << result->printNode();
//    qDebug() << "Result Value: " << result->getValue();

//    disconnect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);
//    disconnect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);

//    if(result->getReturnType() == SimpleNode::ErrorType)
//        return;

//    QStandardItemModel *SymbolTableModel = new QStandardItemModel(this);
//    populateSymbolTableView(&GlobalSymbolTable, SymbolTableModel->invisibleRootItem());
//    ui->SymbolTableView->setModel(SymbolTableModel);

//    ui->lineEdit_2->setText(result->getValue().value<QString>());
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

void R_SimpleInterpreter::populateSymbolTableView(SymbolTable * const symbolTable, QStandardItem *SymbolTableModel)
{
    QVector<SymbolTableEntryPtr> symbolTableEntries = symbolTable->getSymbolTableEntries();
    for(SymbolTableEntryPtr entry : symbolTableEntries)
    {
        QStandardItem *entryItem = new QStandardItem(entry->PrintSymbolType());
        QList<QStandardItem*> row;
        row.append(entryItem);
        row.append(new QStandardItem(entry->PrintToSymbolToString()));
        SymbolTableModel->appendRow(row);
        if(entry->getType() == SymbolTableEntry::SubSymbolTable)
        {
            populateSymbolTableView(dynamic_cast<SymbolTable*>(entry.data()), entryItem);
        }
    }
}
