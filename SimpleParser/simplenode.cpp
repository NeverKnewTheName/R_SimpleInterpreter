#include "simplenode.h"

#include <QVariant>
#include <QDebug>

#include "valuenode.h"
#include "operationnodes.h"
#include "simplesymboltable.h"

#include "simplestack.h"

#include <memory>
#include <vector>

#include "astvisualizer.h"

#include "simplenodevisitor.h"

SimpleNode::SimpleNode()
{
}

QString SimpleNode::getHumanReadableTypeNameToValueType(const Node::ValueTypes type)
{
    QString HumanReadableTypeName("UNDEFINED");
    switch(type)
    {
    case Node::Integer:
        HumanReadableTypeName = QString("Integer");
        break;
    case Node::Double:
        HumanReadableTypeName = QString("Double");
        break;
    case Node::Bool:
        HumanReadableTypeName = QString("Bool");
        break;
    case Node::String:
        HumanReadableTypeName = QString("String");
        break;
    case Node::Void:
        HumanReadableTypeName = QString("Void");
        break;
    case Node::ErrorType:
        HumanReadableTypeName = QString("ErrorType");
        break;
    }

    return HumanReadableTypeName;
}

bool SimpleNode::canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType)
{
    switch(OrigType)
    {
    case Node::Integer:
        //FALLTHROUGH
    case Node::Double:
        //FALLTHROUGH
    case Node::Bool:
        if(NewType == Node::Integer  || NewType == Node::Double || NewType == Node::Bool || NewType == Node::String)
        {
            return true;
        }
        break;
    case Node::String:
        if(NewType == Node::String)
        {
            return true;
        }
        break;
    case Node::Void:
        if(NewType == Node::Void)
        {
            return true;
        }
        break;
    case Node::ErrorType:
    default:
        break;
    }
    return false;
}

SimpleNode::~SimpleNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

Node::NodeType SimpleNode::getNodeType() const
{
    return Node::ERRORNode;
}

Node::ValueTypes SimpleNode::getReturnType() const
{
    return Node::ErrorType;
}

QString SimpleNode::printValue() const
{
    return QString("INVALID VALUE");
}

QString SimpleNode::printNode() const
{
    return QString("{(INVALID NODE):(%1)}").arg(printValue());
}

EOFNode::EOFNode()
{
}

EOFNode::~EOFNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

Node::NodeType EOFNode::getNodeType() const
{
    return Node::EOFNode;
}

Node::ValueTypes EOFNode::getReturnType() const
{
    return Node::ErrorType;
}

QString EOFNode::printValue() const
{
    return QString("EOF");
}

QString EOFNode::printNode() const
{
    return QString("{(EOF):(EOF)}");
}

std::unique_ptr<SimpleNode> EOFNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new EOFNode());
}

void EOFNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<EOFNode>(new EOFNode(*this)));
}


TerminalNode::TerminalNode()
{

}

TerminalNode::~TerminalNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

bool TerminalNode::IsTerminal() const
{
    return true;
}

bool NonTerminalNode::IsTerminal() const
{
    return false;
}

NonTerminalNode::NonTerminalNode()
{

}

NonTerminalNode::~NonTerminalNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

ScopeNode::ScopeNode(const QString &ScopeName)
{
    //ToDO
}

ScopeNode::ScopeNode(QSharedPointer<SimpleSymbolTable> ScopedSymbolTable)
{
    //ToDO
}

ScopeNode::ScopeNode(const ScopeNode &ToCopy)
{
    //ToDO
}

ScopeNode::~ScopeNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

const QSharedPointer<SimpleSymbolTable> &ScopeNode::GetScopeSymbolTable() const
{
    return ScopeSymbolTable;
}

const QString &ScopeNode::getScopeName() const
{
    return ScopeName;
}

void ScopeNode::AddExpressionToScope(std::unique_ptr<SimpleNode> Expression)
{
    //ToDO
}

void ScopeNode::SetScopeReturnType(const Node::ValueTypes &returnType)
{
    //ToDO
}

Node::NodeType ScopeNode::getNodeType() const
{
    return Node::Scope;
}

Node::ValueTypes ScopeNode::getReturnType() const
{
    return ScopeReturnType;
}

QString ScopeNode::printValue() const
{
    return QString("");
}

QString ScopeNode::printNode() const
{
    return QString("{(ScopeNode):()}");
}

std::unique_ptr<SimpleNode> ScopeNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ScopeNode(*this));
}

void ScopeNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<ScopeNode>(new ScopeNode(*this)));
}
