#include "r_simpleinterpreter.h"
#include <QApplication>

#include "simpletoken.h"
#include "simplelexer.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //    ValueToken value1(2);
//    ValueToken value2(6);


    R_SimpleInterpreter w;
    w.show();

    //    return 0;
    return a.exec();
}
