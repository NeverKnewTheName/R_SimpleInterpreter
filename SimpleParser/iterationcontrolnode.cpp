#include "iterationcontrolnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"
#include "simplenodevisitor.h"

#include <QDebug>

IterationControlNode::IterationControlNode(std::unique_ptr<SimpleNode> Statement) :
    IterationStatement(std::move(Statement))
{

}

IterationControlNode::IterationControlNode(const IterationControlNode &ToCopy) :
    IterationStatement(ToCopy.IterationStatement->deepCopy())
{
}

IterationControlNode::~IterationControlNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

void IterationControlNode::replaceStatement(std::unique_ptr<SimpleNode> Statement)
{
    IterationStatement.reset(Statement.release());
}

const std::unique_ptr<SimpleNode> &IterationControlNode::getIterationStatement() const
{
    return IterationStatement;
}

ControlNode::ControlType IterationControlNode::getControlType() const
{
    return ControlNode::ITERATION;
}

ForLoopNode::ForLoopNode(std::unique_ptr<SimpleNode> Initialization, std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> Update, std::unique_ptr<SimpleNode> Statement) :
    IterationControlNode(std::move(Statement)),
    ForLoopInitialization(std::move(Initialization)),
    ForLoopCondition(std::move(Condition)),
    ForLoopUpdate(std::move(Update))
{
}

ForLoopNode::ForLoopNode(const ForLoopNode &ToCopy) :
    IterationControlNode(ToCopy),
    ForLoopInitialization(ToCopy.ForLoopInitialization->deepCopy()),
    ForLoopCondition(ToCopy.ForLoopCondition->deepCopy()),
    ForLoopUpdate(ToCopy.ForLoopUpdate->deepCopy())
{
}

ForLoopNode::~ForLoopNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

Node::ValueTypes ForLoopNode::getReturnType() const
{
    return Node::Void;
}

ControlNode::SpecificControlType ForLoopNode::getSpecificControlType() const
{
    return ControlNode::FOR;
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

WhileLoopNode::WhileLoopNode(std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> Statement) :
    IterationControlNode(std::move(Statement)),
    WhileLoopCondition(std::move(Condition))
{

}

WhileLoopNode::WhileLoopNode(const WhileLoopNode &ToCopy) :
    IterationControlNode(ToCopy),
    WhileLoopCondition(ToCopy.WhileLoopCondition->deepCopy())
{
}

WhileLoopNode::~WhileLoopNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

const std::unique_ptr<SimpleNode> &WhileLoopNode::getWhileCondition()
{
    return WhileLoopCondition;
}

Node::ValueTypes WhileLoopNode::getReturnType() const
{
    return Node::Void;
}

ControlNode::SpecificControlType WhileLoopNode::getSpecificControlType() const
{
    return ControlNode::WHILE;
}

void WhileLoopNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<WhileLoopNode>(new WhileLoopNode(*this)));
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
    return std::unique_ptr<SimpleNode>(new WhileLoopNode(*this));
}


DoWhileLoopNode::DoWhileLoopNode(std::unique_ptr<SimpleNode> Condition, std::unique_ptr<SimpleNode> Statement) :
    WhileLoopNode(std::move(Condition), std::move(Statement))
{
}

DoWhileLoopNode::~DoWhileLoopNode()
{
    #ifdef __DEBUG_OUTPUT__
    qDebug() << __PRETTY_FUNCTION__;
#endif
}

std::unique_ptr<SimpleNode> DoWhileLoopNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DoWhileLoopNode(*this));
}

ControlNode::SpecificControlType DoWhileLoopNode::getSpecificControlType() const
{
    return ControlNode::DOWHILE;
}

void DoWhileLoopNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<DoWhileLoopNode>(new DoWhileLoopNode(*this)));
}

QString DoWhileLoopNode::printValue() const
{
    return QString("do ").append(WhileLoopNode::printValue());
}

QString DoWhileLoopNode::printNode() const
{
    return QString("{(DoWhileLoopNode):(%1)}").arg(printValue());
}

void ForLoopNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ForLoopNode>(new ForLoopNode(*this)));
}
