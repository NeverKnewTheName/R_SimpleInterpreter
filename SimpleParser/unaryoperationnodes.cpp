#include "unaryoperationnodes.h"

#include "simplenode.h"
#include "valuenode.h"

#include "variablenode.h"
#include "programnode.h"
#include "simplestack.h"

#include "simplenodevisitor.h"

UnaryArithmeticOperationNode::UnaryArithmeticOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

TypeCastNode::TypeCastNode(std::unique_ptr<SimpleNode> rightChild, Node::ValueTypes typeToCastTo) :
    UnaryArithmeticOperationNode(std::move(rightChild)),
    typeToCastTo(typeToCastTo)
{
    returnType = UnaryOPRightChild->getReturnType();

    switch(returnType)
    {
    case Node::Integer:
    case Node::Double:
    case Node::Bool:
        if(typeToCastTo != Node::ErrorType )
        {
            implicitCastRightChild = typeToCastTo;
            returnType = typeToCastTo;
        }
        else
        {
            implicitCastRightChild = Node::ErrorType;
            returnType = Node::ErrorType;
        }
        break;
    case Node::String:
        if(typeToCastTo == Node::String)
        {
            implicitCastRightChild = Node::String;
            returnType = Node::String;
        }
        else
        {
            implicitCastRightChild = Node::ErrorType;
            returnType = Node::ErrorType;
        }
        break;
    case Node::ErrorType:
        implicitCastRightChild = Node::ErrorType;
        returnType = Node::ErrorType;
        break;
    }
}

TypeCastNode::TypeCastNode(const TypeCastNode &ToCopy) :
    UnaryArithmeticOperationNode(ToCopy),
    typeToCastTo(ToCopy.typeToCastTo)
{

}

OperationNode::Operation TypeCastNode::getOp() const
{
    return OperationNode::TypeCast;
}

OperationNode::Associativity TypeCastNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence TypeCastNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString TypeCastNode::printValue() const
{
    QString TypeClearName;
    switch(typeToCastTo)
    {
    case Node::Integer:
        TypeClearName = QString("Integer");
        break;
    case Node::Double:
        TypeClearName = QString("Double");
        break;
    case Node::Bool:
        TypeClearName = QString("Bool");
        break;
    case Node::String:
        TypeClearName = QString("String");
        break;
    case Node::ErrorType:
        TypeClearName = QString("ErrorType");
        break;
    }

    return QString("(%1)").arg(TypeClearName);
}

QString TypeCastNode::printNode() const
{
    return QString("{(TypeCast):(%1)}").arg(printValue());
}

std::unique_ptr<SimpleNode> TypeCastNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new TypeCastNode(*this));
}

Node::ValueTypes TypeCastNode::getTypeToCastTo() const
{
    return typeToCastTo;
}

IncrementNode::IncrementNode(std::unique_ptr<SimpleNode> rightChild, const bool isPre) :
    UnaryArithmeticOperationNode(std::move(rightChild)),
    IsPre(isPre)
{
    returnType = UnaryOPRightChild->getReturnType();
    if(returnType == Node::Integer)
    {
        implicitCastRightChild = Node::Integer;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

IncrementNode::IncrementNode(const IncrementNode &ToCopy) :
    UnaryArithmeticOperationNode(ToCopy),
    IsPre(ToCopy.IsPre)
{

}

OperationNode::Operation IncrementNode::getOp() const
{
    return OperationNode::Increment;
}

OperationNode::Associativity IncrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence IncrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString IncrementNode::printValue() const
{
    return QString("++");
}

QString IncrementNode::printNode() const
{
    QString NodeType = "IncrementNode";
    QString value = printValue();

    return QString("{(%1%2):(%4)}").arg(IsPre ? QString("Pre") : QString("Post")).arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> IncrementNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new IncrementNode(*this));
}

bool IncrementNode::getIsPre() const
{
    return IsPre;
}

DecrementNode::DecrementNode(std::unique_ptr<SimpleNode> rightChild, const bool isPre) :
    UnaryArithmeticOperationNode(std::move(rightChild)),
    IsPre(isPre)
{
    returnType = UnaryOPRightChild->getReturnType();
    if(returnType == Node::Integer)
    {
        implicitCastRightChild = Node::Integer;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

DecrementNode::DecrementNode(const DecrementNode &ToCopy) :
    UnaryArithmeticOperationNode(ToCopy),
    IsPre(ToCopy.IsPre)
{

}

OperationNode::Operation DecrementNode::getOp() const
{
    return OperationNode::Decrement;
}

OperationNode::Associativity DecrementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence DecrementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString DecrementNode::printValue() const
{
    return QString("--");
}

QString DecrementNode::printNode() const
{
    QString NodeType = "DecrementNode";
    QString value = printValue();

    return QString("{(%1%2):(%4)}").arg(IsPre ? QString("Pre") : QString("Post")).arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> DecrementNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DecrementNode(*this));
}

bool DecrementNode::getIsPre() const
{
    return IsPre;
}

PositiveNode::PositiveNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( returnType == Node::Integer )
    {
        implicitCastRightChild = Node::Integer;
    }
    else if( returnType == Node::Double )
    {
        implicitCastRightChild = Node::Double;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation PositiveNode::getOp() const
{
    return OperationNode::Positive;
}

OperationNode::Associativity PositiveNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence PositiveNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString PositiveNode::printValue() const
{
    return QString("+");
}

QString PositiveNode::printNode() const
{
    QString NodeType = "PositiveNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> PositiveNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new PositiveNode(*this));
}

NegativeNode::NegativeNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryArithmeticOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( returnType == Node::Integer )
    {
        implicitCastRightChild = Node::Integer;
    }
    else if( returnType == Node::Double )
    {
        implicitCastRightChild = Node::Double;
    }
    else
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation NegativeNode::getOp() const
{
    return OperationNode::Negative;
}

OperationNode::Associativity NegativeNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence NegativeNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString NegativeNode::printValue() const
{
    return QString("-");
}

QString NegativeNode::printNode() const
{
    QString NodeType = "NegativeNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> NegativeNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new NegativeNode(*this));
}

UnaryLogicalOperationNode::UnaryLogicalOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

std::unique_ptr<SimpleNode> UnaryLogicalOperationNode::deepCopy() const
{

}

LogicalNegationNode::LogicalNegationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryLogicalOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( ( returnType != Node::Integer ) && ( returnType != Node::Double )  && ( returnType != Node::Bool ) )
        returnType = Node::ErrorType;
}

OperationNode::Operation LogicalNegationNode::getOp() const
{
    return OperationNode::LogicalNegation;
}

OperationNode::Associativity LogicalNegationNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence LogicalNegationNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString LogicalNegationNode::printValue() const
{
    return QString("!");
}

QString LogicalNegationNode::printNode() const
{
    QString NodeType = "LogicalNegationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LogicalNegationNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LogicalNegationNode(*this));
}

UnaryBitwiseOperationNode::UnaryBitwiseOperationNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryOperationNode(std::move(rightChild))
{
}

OperationNode::OperationTypes UnaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

std::unique_ptr<SimpleNode> UnaryBitwiseOperationNode::deepCopy() const
{

}

OnesComplementNode::OnesComplementNode(std::unique_ptr<SimpleNode> rightChild) :
    UnaryBitwiseOperationNode(std::move(rightChild))
{
    returnType = UnaryOPRightChild->getReturnType();
    if( ( returnType != Node::Integer ) )
    {
        returnType = Node::ErrorType;
    }
}

OperationNode::Operation OnesComplementNode::getOp() const
{
    return OperationNode::OnesComplement;
}

OperationNode::Associativity OnesComplementNode::getAssociativity() const
{
    return OperationNode::RightToLeft;
}

OperationNode::Precedence OnesComplementNode::getPrecedence() const
{
    return OperationNode::UnaryPrec;
}

QString OnesComplementNode::printValue() const
{
    return QString("~");
}

QString OnesComplementNode::printNode() const
{
    QString NodeType = "OnesComplementNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> OnesComplementNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new OnesComplementNode(*this));
}



void TypeCastNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<TypeCastNode>(new TypeCastNode(*this)));
}


void IncrementNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<IncrementNode>(new IncrementNode(*this)));
}


void DecrementNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<DecrementNode>(new DecrementNode(*this)));
}

void PositiveNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<PositiveNode>(new PositiveNode(*this)));
}

void NegativeNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<NegativeNode>(new NegativeNode(*this)));
}

void LogicalNegationNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<LogicalNegationNode>(new LogicalNegationNode(*this)));
}

void OnesComplementNode::accept(SimpleNodeVisitor *visitor) const
{
    visitor->visit(std::unique_ptr<OnesComplementNode>(new OnesComplementNode(*this)));
}
