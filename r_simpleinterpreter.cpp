#include "r_simpleinterpreter.h"
#include "ui_r_simpleinterpreter.h"

#include "simplelexer.h"
#include "simpleparser.h"
#include "simpleinterpreter.h"

#include "simpletoken.h"

#include "simplenode.h"
#include "valuenode.h"

#include "astvisualizer.h"

#include "simplesymboltableentry.h"
#include "simplesymboltable.h"
#include "functionsymbol.h"
#include "variablesymbol.h"


#include "simpleastinterpretervisitor.h"
#include "astvisualizervisitor.h"

#include <QMessageBox>
#include <QThread>
#include <QDebug>

#include <QStandardItemModel>
#include <QStandardItem>

#include <QGraphicsScene>

#include <QPrinter>
#include <QPrintDialog>

R_SimpleInterpreter::R_SimpleInterpreter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::R_SimpleInterpreter),
    GlobalSymbolTable( new SimpleSymbolTable(QString("GlobalSymblTbl")))
{
    ui->setupUi(this);
    ui->textEdit->hide();
    QSharedPointer<ConstantValueSymbol> newConstValSymbol(new ConstantValueSymbol(QString("D0"), ValueNode(10)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D1"), ValueNode(20)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D2"), ValueNode(30)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D3"), ValueNode(40)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D4"), ValueNode(50)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D5"), ValueNode(60)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D6"), ValueNode(70)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("D7"), ValueNode(80)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

    newConstValSymbol = QSharedPointer<ConstantValueSymbol>(new ConstantValueSymbol(QString("xxVARxx"), ValueNode(1)));
    GlobalSymbolTable->addEntry(newConstValSymbol);

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

    SimpleASTInterpreterVisitor astInterpreter;

    std::unique_ptr<SimpleNode> parseTree = std::move(parse.ParseToAST());

    if(parseTree == nullptr)
    {
        qDebug() << "Error in ParseTree";
        return;
    }
    astInterpreter.visit(parseTree->deepCopy());
    std::unique_ptr<ValueNode> result = astInterpreter.getInterpreterResult();
    if(result != nullptr)
    {
        qDebug() << "Result Node: " << result->printNode();
        qDebug() << "Result Value: " << result->getValue();
    }
    disconnect(lex, &SimpleLexer::LexerErrorHTMLMsg, this, &R_SimpleInterpreter::receiveLexerHTMLFormattedErrMsg);
    disconnect(lex, &SimpleLexer::LexerPosAt, this, &R_SimpleInterpreter::lexerPosChanged);

    if((result == nullptr) || (result->getReturnType() == Node::ErrorType))
        return;

    QStandardItemModel *SymbolTableModel = new QStandardItemModel(this);
    populateSymbolTableView(GlobalSymbolTable, SymbolTableModel->invisibleRootItem());
    ui->SymbolTableView->setModel(SymbolTableModel);

    ASTVisualizerVisitor ASTVisualizer;

    ASTVisualizer.visit(parseTree->deepCopy());

    ASTNode *rootASTNode = ASTVisualizer.getVisualizedASTRootNode();
    QGraphicsScene *myScene = new QGraphicsScene();
    myScene->addItem(rootASTNode);
    ui->graphicsView->setScene(myScene);

//#define _PRINT_AST_
#ifdef _PRINT_AST_
    QPrinter printer;
    if (QPrintDialog(&printer).exec() == QDialog::Accepted) {
        printer.setPageOrientation(QPageLayout::Landscape);
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        myScene->render(&painter);
    }
#endif

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

void R_SimpleInterpreter::populateSymbolTableView(QSharedPointer<SimpleSymbolTable> symbolTable, QStandardItem *SymbolTableModel)
{
    std::vector<QSharedPointer<SimpleSymbolTableEntry>> symbolTableEntries = symbolTable->getSymbolTableEntries();
    for(const QSharedPointer<SimpleSymbolTableEntry> &entry : symbolTableEntries)
    {
        QStandardItem *entryItem = new QStandardItem(entry->PrintSymbolType());
        QList<QStandardItem*> row;
        row.append(entryItem);
        row.append(new QStandardItem(entry->PrintToSymbolToString()));
        SymbolTableModel->appendRow(row);
        if(entry->getType() == SimpleSymbolTableEntry::SubSymbolTable)
        {
            populateSymbolTableView(qSharedPointerDynamicCast<SimpleSymbolTable>(entry), entryItem);
        }
        else if(entry->getType() == SimpleSymbolTableEntry::Function)
        {
            populateSymbolTableView(qSharedPointerDynamicCast<FunctionSymbol>(entry)->getFunctionSymbolTable(), entryItem);
        }
    }
}
