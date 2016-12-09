#include "iterationcontrolnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"
#include "simplenodevisitor.h"

#include <QDebug>

IterationControlNode::IterationControlNode()
{

}

IterationControlNode::~IterationControlNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ControlNode::ControlType IterationControlNode::getControlType() const
{
    return ControlNode::ITERATION;
}

ForLoopNode::ForLoopNode(std::unique_ptr<SimpleNode> Initialization, std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> Update, std::vector<std::unique_ptr<SimpleNode> > &Expressions) :
    ForLoopInitialization(std::move(Initialization)),
    ForLoopCondition(std::move(Condition)),
    ForLoopUpdate(std::move(Update)),
    ForLoopExpressions(std::move(Expressions))
{
}

ForLoopNode::ForLoopNode(const ForLoopNode &ToCopy) :
    ForLoopInitialization(ToCopy.ForLoopInitialization->deepCopy()),
    ForLoopCondition(ToCopy.ForLoopCondition->deepCopy()),
    ForLoopUpdate(ToCopy.ForLoopUpdate->deepCopy())
{
    for(const std::unique_ptr<SimpleNode> &exprToCopy : ToCopy.ForLoopExpressions)
    {
        this->ForLoopExpressions.emplace_back(exprToCopy->deepCopy());
    }
}

ForLoopNode::~ForLoopNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::ValueTypes ForLoopNode::getReturnType() const
{
    return Node::Void;
}


ControlNode::SpecificControlType ForLoopNode::getSpecificControlType() const
{
    return ControlNode::FOR;
}

const std::vector<std::unique_ptr<SimpleNode> > &ForLoopNode::getForLoopExpressions() const
{
    return ForLoopExpressions;
}

const std::unique_ptr<SimpleNode> &ForLoopNode::getForLoopUpdate() const
{
    return ForLoopUpdate;
}

const std::unique_ptr<SimpleNode> &ForLoopNode::getForLoopCondition() const
{
    return ForLoopCondition;
}

const std::unique_ptr<SimpleNode> &ForLoopNode::getForLoopInitialization() const
{
    return ForLoopInitialization;
}

void ForLoopNode::executeLoop(QSharedPointer<SimpleStack> StackToUse) const
{

}

QString ForLoopNode::printValue() const
{
    return QString("for(%1;%2;%3)").arg(ForLoopInitialization->printNode()).arg(ForLoopCondition->printNode()).arg(ForLoopUpdate->printNode());
}

QString ForLoopNode::printNode() const
{
    return QString("{(ForLoopNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> ForLoopNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ForLoopNode(*this));
}

WhileLoopNode::WhileLoopNode(std::unique_ptr<SimpleNode> Condition, std::vector<std::unique_ptr<SimpleNode> > &Expressions) :
    WhileLoopCondition(std::move(Condition)),
    WhileLoopExpressions(std::move(Expressions))
{

}

WhileLoopNode::WhileLoopNode(const WhileLoopNode &ToCopy) :
    WhileLoopCondition(ToCopy.WhileLoopCondition->deepCopy())
{
    for(const std::unique_ptr<SimpleNode> &expr : ToCopy.WhileLoopExpressions)
    {
        this->WhileLoopExpressions.emplace_back(expr->deepCopy());
    }
}

WhileLoopNode::~WhileLoopNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::ValueTypes WhileLoopNode::getReturnType() const
{
    return Node::Void;
}

ControlNode::SpecificControlType WhileLoopNode::getSpecificControlType() const
{
    return ControlNode::WHILE;
}

void WhileLoopNode::executeLoop(QSharedPointer<SimpleStack> StackToUse) const
{

}

QString WhileLoopNode::printValue() const
{
    return QString("while(%1)").arg(WhileLoopCondition->printNode());
}

QString WhileLoopNode::printNode() const
{
    return QString("{(WhileLoopNode):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> WhileLoopNode::deepCopy() const
{
//    return std::unique_ptr<SimpleNode>(new WhileLoopNode(*this));
}


DoWhileLoopNode::DoWhileLoopNode(std::unique_ptr<SimpleNode> Condition, std::vector<std::unique_ptr<SimpleNode> > &Expressions) :
    WhileLoopNode(std::move(Condition), Expressions)
{

}

DoWhileLoopNode::~DoWhileLoopNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

std::unique_ptr<SimpleNode> DoWhileLoopNode::deepCopy() const
{
//    return std::unique_ptr<SimpleNode>(new DoWhileLoopNode(*this));
}

ControlNode::SpecificControlType DoWhileLoopNode::getSpecificControlType() const
{
    return ControlNode::DOWHILE;
}

QString DoWhileLoopNode::printValue() const
{
    return QString("do...").append(WhileLoopNode::printValue());
}

QString DoWhileLoopNode::printNode() const
{
    return QString("{(DoWhileLoopNode):(%1)}").arg(printValue());
}


void ForLoopNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ForLoopNode>(new ForLoopNode(*this)));
}
