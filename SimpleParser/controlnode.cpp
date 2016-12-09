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
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType ControlNode::getNodeType() const
{
    return Node::Control;
}

