#include "selectioncontrolnode.h"

#include "simplenodevisitor.h"

#include "valuenode.h"

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
    IfStatementBlock(std::move(StatementBlock)),
    IfElseNode(new ValueNode())
{
}

IfNode::IfNode(const IfNode &ToCopy) :
    SelectionControlNode(ToCopy),
    IfCondition(ToCopy.IfCondition->deepCopy()),
    IfStatementBlock(ToCopy.IfStatementBlock->deepCopy()),
    IfElseNode(ToCopy.IfElseNode->deepCopy())
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

void IfNode::addElse(std::unique_ptr<ElseNode> ElseNode)
{
    IfElseNode.reset(ElseNode.release());
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

const std::unique_ptr<SimpleNode> &IfNode::getIfElseNode() const
{
    return IfElseNode;
}


ElseNode::ElseNode(std::unique_ptr<SimpleNode> StatementBlock) :
    ElseStatementBlock(std::move(StatementBlock))
{

}

ElseNode::ElseNode(const ElseNode &ToCopy) :
    SelectionControlNode(ToCopy),
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
    returnType(Node::Void),
    SwitchCondition(std::move(Condition))
{

}

SwitchNode::SwitchNode(const SwitchNode &ToCopy) :
    SelectionControlNode(ToCopy),
    returnType(ToCopy.returnType),
    SwitchCondition(ToCopy.SwitchCondition->deepCopy())
{
    const std::vector<std::unique_ptr<SwitchLabel>> &SwitchLabels = ToCopy.getSwitchLabels();
    for(const std::unique_ptr<SwitchLabel> &label : SwitchLabels)
    {
        this->SwitchLabels.emplace_back(std::unique_ptr<SwitchLabel>(dynamic_cast<SwitchLabel*>(label->deepCopy().release())));
    }
}

SwitchNode::~SwitchNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &SwitchNode::getSwitchCondition() const
{
    return SwitchCondition;
}

void SwitchNode::addSwitchLabel(std::unique_ptr<SwitchLabel> SwitchLabel)
{
    SwitchLabels.emplace_back(std::move(SwitchLabel));
}

void SwitchNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<SwitchNode>(new SwitchNode(*this)));
}

Node::ValueTypes SwitchNode::getReturnType() const
{
    return returnType;
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

const std::vector<std::unique_ptr<SwitchLabel> > &SwitchNode::getSwitchLabels() const
{
    return SwitchLabels;
}


CaseNode::CaseNode(std::unique_ptr<ValueNode> Argument, std::unique_ptr<SimpleNode> Statement) :
    SwitchLabel(std::move(Statement)),
    CaseArgument(std::move(Argument))
{

}

CaseNode::CaseNode(const CaseNode &ToCopy) :
    SwitchLabel(ToCopy),
    CaseArgument(dynamic_cast<ValueNode*>(ToCopy.CaseArgument->deepCopy().release()))
{

}

CaseNode::~CaseNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<ValueNode> &CaseNode::getCaseArgument() const
{
    return CaseArgument;
}

void CaseNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<CaseNode>(new CaseNode(*this)));
}


QString CaseNode::printValue() const
{
    return QString("case %1:").arg(CaseArgument->printValue());
}

QString CaseNode::printNode() const
{
    return QString("{(CaseNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> CaseNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new CaseNode(*this));
}

DefaultNode::DefaultNode(std::unique_ptr<SimpleNode> Statement) :
    SwitchLabel(std::move(Statement))
{

}

DefaultNode::DefaultNode(const DefaultNode &ToCopy) :
    SwitchLabel(ToCopy)
{

}

DefaultNode::~DefaultNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

QString DefaultNode::printValue() const
{
    return QString("default:");
}

QString DefaultNode::printNode() const
{
    return QString("{(DefaultNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> DefaultNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DefaultNode(*this));
}

SwitchLabel::SwitchLabel(std::unique_ptr<SimpleNode> Statement) :
    returnType(Statement->getReturnType()),
    SwitchLabelStatement(std::move(Statement))
{

}

SwitchLabel::SwitchLabel(const SwitchLabel &ToCopy) :
    returnType(ToCopy.returnType),
    SwitchLabelStatement(ToCopy.SwitchLabelStatement->deepCopy())
{

}

SwitchLabel::~SwitchLabel()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const std::unique_ptr<SimpleNode> &SwitchLabel::getSwitchLabelStatement() const
{
    return SwitchLabelStatement;
}

Node::ValueTypes SwitchLabel::getReturnType() const
{
    return returnType;
}


void DefaultNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<DefaultNode>(new DefaultNode(*this)));
}

Node::NodeType DefaultNode::getNodeType() const
{
    return Node::Label;
}

SwitchLabel::SwitchLabelType DefaultNode::getSwitchLabelType() const
{
    return SwitchLabel::DefaultLable;
}

Node::NodeType CaseNode::getNodeType() const
{
    return Node::Label;
}

SwitchLabel::SwitchLabelType CaseNode::getSwitchLabelType() const
{
    return SwitchLabel::CaseLabel;
}
