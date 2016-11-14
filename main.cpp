#include "r_simpleinterpreter.h"
#include <QApplication>

#include "simpletoken.h"
#include "simplelexer.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    R_SimpleInterpreter w;
    w.show();

    return a.exec();
}
