#ifndef R_SIMPLEINTERPRETER_H
#define R_SIMPLEINTERPRETER_H

#include <QMainWindow>

#include <QStandardItem>

#include <simplesymboltable.h>

class SimpleLexer;
class SimpleParser;
class SimpleInterpreter;


namespace Ui {
class R_SimpleInterpreter;
}

class R_SimpleInterpreter : public QMainWindow
{
    Q_OBJECT

public:
    explicit R_SimpleInterpreter(QWidget *parent = 0);
    ~R_SimpleInterpreter();

private slots:
    void on_pushButton_clicked();
    void lexerPosChanged(int pos, int len);
    void receiveLexerHTMLFormattedErrMsg(QString HTMLFormattedErrMsg);
    void on_SaveSourceBtn_clicked();

    void on_LoadSourceBtn_clicked();

private:
    void populateSymbolTableView(QSharedPointer<SimpleSymbolTable> symbolTable, QStandardItem *SymbolTableModel);

private:
    Ui::R_SimpleInterpreter *ui;
    SimpleLexer *lex;
    SimpleParser *parse;
    SimpleInterpreter *interpr;
    QSharedPointer<SimpleSymbolTable> GlobalSymbolTable;
};

#endif // R_SIMPLEINTERPRETER_H
