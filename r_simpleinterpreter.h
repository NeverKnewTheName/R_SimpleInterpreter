#ifndef R_SIMPLEINTERPRETER_H
#define R_SIMPLEINTERPRETER_H

#include <QMainWindow>

#include <QStandardItem>

#include <symboltable.h>

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

private:
    void populateSymbolTableView(SymbolTable * const symbolTable, QStandardItem *SymbolTableModel);

private:
    Ui::R_SimpleInterpreter *ui;
    SimpleLexer *lex;
    SimpleParser *parse;
    SimpleInterpreter *interpr;
    QSharedPointer<SymbolTable> GlobalSymbolTable;
};

#endif // R_SIMPLEINTERPRETER_H
