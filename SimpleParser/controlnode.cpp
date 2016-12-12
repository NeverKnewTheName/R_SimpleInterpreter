#include "controlnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

#include "astvisualizer.h"

#include <QDebug>

ControlNode::ControlNode()
{

}

ControlNode::~ControlNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

Node::NodeType ControlNode::getNodeType() const
{
    return Node::Control;
}

