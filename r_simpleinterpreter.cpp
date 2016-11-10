#include "r_simpleinterpreter.h"
#include "ui_r_simpleinterpreter.h"

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
    QString TextToInterpret = ui->lineEdit->text().simplified();
    int indexOfWs = TextToInterpret.indexOf(' ');
    while(indexOfWs != -1)
    {
        TextToInterpret.remove(indexOfWs,1);
        indexOfWs = TextToInterpret.indexOf(' ');
    }
    qDebug() << TextToInterpret;
}
