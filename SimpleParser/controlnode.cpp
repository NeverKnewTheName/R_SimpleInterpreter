#include "controlnode.h"

#include "valuenode.h"
#include "simplesymboltable.h"
#include "simplestack.h"

#include "astvisualizer.h"

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



ScopedControlNode::ScopedControlNode(const QString &ScopeName) :
    ScopeName(QString(ScopeName).append("%1").arg(ScopeCntr++)),
    ScopeSymbolTable(new SimpleSymbolTable(this->ScopeName)),
    ScopeReturnType(Node::Void)
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

std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > ScopedControlNode::FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const
{
    //ToDO need to increment maxStackSize... Decrement also sometimes... //ToTHINK
    return std::move(FlatAST);
}

void ScopedControlNode::setScopeReturnType(const Node::ValueTypes &value)
{
    ScopeReturnType = value;
}

Node::ValueTypes ScopedControlNode::getReturnType() const
{
    return ScopeReturnType;
}

const QString &ScopedControlNode::getScopeName() const
{
    return ScopeName;
}

void ScopedControlNode::AddScopeExpression(std::unique_ptr<SimpleNode> Expression)
{
    ScopeExpressions.emplace_back(std::move(Expression));
}

ASTNode *ScopedControlNode::VisualizeNode(ASTNode *parentNode) const
{
    ASTNode *ScopeASTNode = new ASTNode(ScopeName, parentNode);
    new ASTNode(QString("{"), ScopeASTNode);
    for(const std::unique_ptr<SimpleNode> &expression : ScopeExpressions)
    {
        expression->VisualizeNode(ScopeASTNode);
        new ASTNode(QString(";"),ScopeASTNode);
    }
    new ASTNode(QString("}"), ScopeASTNode);

    return ScopeASTNode;
}

std::unique_ptr<ValueNode> ScopedControlNode::visit(QSharedPointer<SimpleStack> StackToUse) const
{
    ScopeSymbolTable->EnterScope(StackToUse);
    for(const std::unique_ptr<SimpleNode> &expression : ScopeExpressions)
    {
        expression->visit(StackToUse);
    }
    ScopeSymbolTable->ExitScope(StackToUse);
}
