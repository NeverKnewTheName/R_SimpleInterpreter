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
    switch(type & Node::ValType)
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
    // IS either one an error?
    if( OrigType == Node::ErrorType || NewType == Node::ErrorType )
    {
        qDebug() << "ErrorType cannot be converted at all!";
        return false;
    }
    // IS it a conversion from unsigned to signed?
    if( ( OrigType & Node::SignBit ) != ( NewType & Node::SignBit ) )
    {
        qDebug() << "Type SIGN mismatch!";
        return false;
    }
    // IS it a conversion from a scalar to to a non-scalar type or vice versa?
    if( (OrigType & Node::NonScalarBit) != ( NewType & Node::NonScalarBit ) )
    {
        qDebug() << "Cannot convert scalar type to non-scalar type!";
        return false;
    }
    // IS Either one Void?
    if( ( OrigType & Node::Void ) ^ ( NewType & Node::Void ) )
    {
        qDebug() << "Cannot convert from or to Void!";
        return false;
    }

    // IS it of the same type but different amount of used bits?
    if( OrigType & NewType & Node::ValType)
    {
        if((NewType & Node::BitCnt) > (OrigType & Node::BitCnt))
        {
            qDebug() << "NewType uses less bits than OrigType!";
            return false;
        }
    }

    // IS it a user defined type?
    if( NewType & Node::UserType )
    {
        if( NewType == OrigType )
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    // CAN convert types?
    if( ( NewType & Node::ValType ) > ( OrigType & Node::ValType ) )
    {
        return false;
    }

    return true;

//    switch(OrigType & Node::ValType)
//    {
//    case Node::Integer:
//        //FALLTHROUGH
//    case Node::Double:
//        //FALLTHROUGH
//    case Node::Bool:
//        if((NewType & Node::ValType) == Node::Integer  || (NewType & Node::ValType) == Node::Double || (NewType & Node::ValType) == Node::Bool || (NewType & Node::ValType) == Node::String)
//        {
//            return true;
//        }
//        break;
//    case Node::String:
//        if((NewType & Node::ValType) == Node::String)
//        {
//            return true;
//        }
//        break;
//    case Node::Void:
//        if((NewType & Node::ValType) == Node::Void)
//        {
//            return true;
//        }
//        break;
//    case Node::ErrorType:
//    default:
//        break;
//    }
//    return false;
}

Node::ValueTypes SimpleNode::evaluateResultingType(const Node::ValueTypes type1, const Node::ValueTypes type2)
{
    quint64 resultingType = ((quint64)type1 | (quint64)type2);
    quint64 typeExtension = resultingType & Node::TypeExtens;
    resultingType &=  Node::TypeDecl;

    if( ( type1 & Node::SignBit ) != ( type2 & Node::SignBit ) )
    {
        qDebug() << "Type SIGN mismatch!";
        return Node::ErrorType;
    }
    if( (type1 & Node::NonScalarBit) != ( type2 & Node::NonScalarBit ) )
    {
        qDebug() << "Cannot convert scalar type to non-scalar type!";
        return Node::ErrorType;
    }
    if( ( type1 & Node::Void ) || ( type2 & Node::Void ) )
    {
        qDebug() << "Cannot convert from or to Void!";
        return Node::ErrorType;
    }

    resultingType = resultingType & (-resultingType) | typeExtension;

    return (Node::ValueTypes)resultingType;
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
