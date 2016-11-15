#include "simpleinterpreter.h"

#include "simpleparser.h"
#include "simpleast.h"

#include <QDebug>

SimpleInterpreter::SimpleInterpreter(SimpleParser *parser) :
    parser(parser)
{
    tree = parser->parse();
}

SimpleInterpreter::~SimpleInterpreter()
{
    qDebug() << __PRETTY_FUNCTION__;
    if(tree != NULL)
        delete tree;
}

ValueNode *SimpleInterpreter::interpret()
{
    return &tree->visit();
}
