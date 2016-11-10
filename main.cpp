#include "r_simpleinterpreter.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    R_SimpleInterpreter w;
    w.show();

    return a.exec();
}
