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
#include "blocknode.h"
#include "selectioncontrolnode.h"
#include "iterationcontrolnode.h"
#include "escapecontrolnode.h"


#include <QDebug>

SimpleNodeVisitor::SimpleNodeVisitor()
{

}

SimpleNodeVisitor::~SimpleNodeVisitor()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}
