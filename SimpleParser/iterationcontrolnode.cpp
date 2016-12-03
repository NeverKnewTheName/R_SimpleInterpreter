#include "iterationcontrolnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

IterationControlNode::IterationControlNode()
{

}

bool IterationControlNode::EnterScope(QSharedPointer<SimpleStack> StackToUse) const
{
    return ScopedControlNode::EnterScope(StackToUse);
}

bool IterationControlNode::ExitScope(QSharedPointer<SimpleStack> StackToUse) const
{
    return ScopedControlNode::ExitScope(StackToUse);
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

bool ForLoopNode::EnterScope(QSharedPointer<SimpleStack> StackToUse) const
{
    return IterationControlNode::EnterScope(StackToUse);
}

bool ForLoopNode::ExitScope(QSharedPointer<SimpleStack> StackToUse) const
{
    return IterationControlNode::EnterScope(StackToUse);
}

Node::ValueTypes ForLoopNode::getReturnType() const
{
    return Node::Void;
}

std::unique_ptr<ValueNode> ForLoopNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{

}

ControlNode::SpecificControlType ForLoopNode::getSpecificControlType() const
{
    return ControlNode::FOR;
}

QString ForLoopNode::printValue() const
{
    return QString("for(%1;%2;%3)").arg(ForLoopInitialization->printNode()).arg(ForLoopCondition->printNode()).arg(ForLoopUpdate->printNode());
}

QString ForLoopNode::printNode() const
{
    return QString("{(ForLoopNode):(%1)}").arg(printValue());
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

Node::ValueTypes WhileLoopNode::getReturnType() const
{
    return Node::Void;
}

std::unique_ptr<ValueNode> WhileLoopNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{

}

ControlNode::SpecificControlType WhileLoopNode::getSpecificControlType() const
{
    return ControlNode::WHILE;
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


DoWhileLoopNode::DoWhileLoopNode(std::unique_ptr<SimpleNode> Condition, std::vector<std::unique_ptr<SimpleNode> > &Expressions) :
    WhileLoopNode(std::move(Condition), Expressions)
{

}

std::unique_ptr<SimpleNode> DoWhileLoopNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DoWhileLoopNode(*this));
}

std::unique_ptr<ValueNode> DoWhileLoopNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    WhileLoopNode::executeLoop(StackToUse);
    return WhileLoopNode::visit(StackToUse);
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
