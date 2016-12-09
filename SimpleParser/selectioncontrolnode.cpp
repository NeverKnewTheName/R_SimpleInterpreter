#include "selectioncontrolnode.h"

#include "simplenodevisitor.h"

#include <QDebug>

SelectionControlNode::SelectionControlNode()
{
}

SelectionControlNode::SelectionControlNode(const SelectionControlNode &ToCopy)
{
}

SelectionControlNode::~SelectionControlNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ControlNode::ControlType SelectionControlNode::getControlType() const
{
    return ControlNode::SELECTION;
}


IfNode::IfNode(std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> StatementBlock) :
    IfCondition(std::move(Condition)),
    IfStatementBlock(std::move(StatementBlock))
{
}

IfNode::IfNode(const IfNode &ToCopy) :
    IfCondition(ToCopy.IfCondition->deepCopy()),
    IfStatementBlock(ToCopy.IfStatementBlock->deepCopy())
{
}

IfNode::~IfNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &IfNode::getIfCondition() const
{
    return IfCondition;
}

const std::unique_ptr<SimpleNode> &IfNode::getIfStatementBlock() const
{
    return IfStatementBlock;
}

void IfNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<IfNode>(new IfNode(*this)));
}

Node::ValueTypes IfNode::getReturnType() const
{
    return Node::Void;
}

QString IfNode::printValue() const
{
    return QString("if");
}

QString IfNode::printNode() const
{
    return QString("{(IfNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> IfNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new IfNode(*this));
}

ControlNode::SpecificControlType IfNode::getSpecificControlType() const
{
    return ControlNode::IF;
}


ElseNode::ElseNode(std::unique_ptr<SimpleNode> StatementBlock) :
    ElseStatementBlock(std::move(StatementBlock))
{

}

ElseNode::ElseNode(const ElseNode &ToCopy) :
    ElseStatementBlock(ToCopy.ElseStatementBlock->deepCopy())
{

}

ElseNode::~ElseNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &ElseNode::getElseStatementBlock() const
{
    return ElseStatementBlock;
}

void ElseNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ElseNode>(new ElseNode(*this)));
}

Node::ValueTypes ElseNode::getReturnType() const
{
    return ElseStatementBlock->getReturnType();
}

QString ElseNode::printValue() const
{
    return QString("else");
}

QString ElseNode::printNode() const
{
    return QString("{(ElseNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> ElseNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ElseNode(*this));
}

ControlNode::SpecificControlType ElseNode::getSpecificControlType() const
{
    return ControlNode::ELSE;
}

SwitchNode::SwitchNode(std::unique_ptr<SimpleNode> Condition) :
    SwitchCondition(std::move(Condition))
{

}

SwitchNode::SwitchNode(const SwitchNode &ToCopy) :
    SwitchCondition(ToCopy.SwitchCondition->deepCopy())
{

}

SwitchNode::~SwitchNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &SwitchNode::getSwitchCondition() const
{
    return SwitchCondition;
}

void SwitchNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<SwitchNode>(new SwitchNode(*this)));
}

Node::ValueTypes SwitchNode::getReturnType() const
{

}

QString SwitchNode::printValue() const
{
    return QString("switch");
}

QString SwitchNode::printNode() const
{
    return QString("{(SwitchNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> SwitchNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new SwitchNode(*this));
}

ControlNode::SpecificControlType SwitchNode::getSpecificControlType() const
{
    return ControlNode::SWITCH;
}
