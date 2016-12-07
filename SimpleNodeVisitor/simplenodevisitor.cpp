#include "simplenodevisitor.h"

#include "simplenode.h"
#include "assignmentnode.h"
#include "expressionnode.h"
#include "functioncallnode.h"
#include "programnode.h"
#include "statementnode.h"
#include "valuenode.h"
#include "variablenode.h"
#include "unaryoperationnodes.h"
#include "binaryoperationnodes.h"
#include "ternaryoperationnodes.h"

#include <QDebug>

SimpleNodeVisitor::SimpleNodeVisitor()
{

}

SimpleNodeVisitor::~SimpleNodeVisitor()
{
    qDebug() << __PRETTY_FUNCTION__;
}
