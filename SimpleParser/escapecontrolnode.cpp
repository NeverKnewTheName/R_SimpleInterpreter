#include "escapecontrolnode.h"

#include "simplenodevisitor.h"

#include <QDebug>

EscapeControlNode::EscapeControlNode()
{

}

EscapeControlNode::~EscapeControlNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

ControlNode::ControlType EscapeControlNode::getControlType() const
{
    return ControlNode::ESCAPE;
}

Node::ValueTypes EscapeControlNode::getReturnType() const
{
    return Node::Void;
}

BreakNode::BreakNode()
{

}

BreakNode::BreakNode(const BreakNode &ToCopy)
{

}

BreakNode::~BreakNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

void BreakNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<BreakNode>(new BreakNode(*this)));
}

QString BreakNode::printValue() const
{
    return QString("break");
}

QString BreakNode::printNode() const
{
    return QString("{(BreakNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> BreakNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new BreakNode(*this));
}

ControlNode::SpecificControlType BreakNode::getSpecificControlType() const
{
    return ControlNode::BREAK;
}

ContinueNode::ContinueNode()
{

}

ContinueNode::ContinueNode(const ContinueNode &ToCopy)
{

}

ContinueNode::~ContinueNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

void ContinueNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ContinueNode>(new ContinueNode(*this)));
}

QString ContinueNode::printValue() const
{
    return QString("continue");
}

QString ContinueNode::printNode() const
{
    return QString("{(ContinueNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> ContinueNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ContinueNode(*this));
}

ControlNode::SpecificControlType ContinueNode::getSpecificControlType() const
{
    return ControlNode::CONTINUE;
}

ReturnNode::ReturnNode(std::unique_ptr<SimpleNode> ReturnExpression) :
    ReturnExpression(std::move(ReturnExpression))
{
}

ReturnNode::ReturnNode(const ReturnNode &ToCopy) :
    ReturnExpression(ToCopy.ReturnExpression->deepCopy())
{
}

ReturnNode::~ReturnNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

void ReturnNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ReturnNode>(new ReturnNode(*this)));
}

QString ReturnNode::printValue() const
{
    return QString("return");
}

QString ReturnNode::printNode() const
{
    return QString("{(ReturnNode):(%1)}").arg(printValue());
}

Node::ValueTypes ReturnNode::getReturnType() const
{
    return ReturnExpression->getReturnType();
}

std::unique_ptr<SimpleNode> ReturnNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ReturnNode(*this));
}

ControlNode::SpecificControlType ReturnNode::getSpecificControlType() const
{
    return ControlNode::RETURN;
}

const std::unique_ptr<SimpleNode> &ReturnNode::getReturnExpression() const
{
    return ReturnExpression;
}
