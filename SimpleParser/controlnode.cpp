#include "controlnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

#include <QDebug>

unsigned int ScopedControlNode::ScopeCntr = 0;

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

ControlNode::ControlType EscapeControlNode::getControlType() const
{
    return ControlNode::ESCAPE;
}



ScopedControlNode::ScopedControlNode() :
    ScopeSymbolTable(new SimpleSymbolTable(QString("ControlScope%1").arg(ScopeCntr++)))
{
}

ScopedControlNode::ScopedControlNode(const ScopedControlNode &ToCopy) :
    ScopeSymbolTable(ToCopy.ScopeSymbolTable)
{

}

ScopedControlNode::~ScopedControlNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const QSharedPointer<SimpleSymbolTable> &ScopedControlNode::getScopedControlNodeSymbolTable() const
{
    return ScopeSymbolTable;
}

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > ScopedControlNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const
{
    //ToDO need to increment maxStackSize... Decrement also sometimes... //ToTHINK
}
