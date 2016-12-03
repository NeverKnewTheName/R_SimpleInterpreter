#include "binaryoperationnodes.h"

#include "valuenode.h"
#include "simplestack.h"

BinaryArithmeticOperationNode::BinaryArithmeticOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOperationNode(std::move(leftChild), std::move(rightChild))
{
}

OperationNode::OperationTypes BinaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

AdditionNode::AdditionNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryArithmeticOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer + Integer -> Integer
         * (implicit cast to) Integer + Double  -> Double
         */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Integer;
        }
        else if( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Double:
        /*
         * Double + Double   -> Double
         * Double + (implicit cast to) Integer  -> Double
         */
        if( ( returnTypeRChild == Node::Double ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Bool: // Bool + ??? does not make sense
        break;
    case Node::String:
        /*
         * String + String  -> Concatenation
         * String + Integer/Double -> String + String(Integer/Double)
         * String + Bool -> String + Bool ? String("true") : String("false")
         */
        if( returnTypeRChild != Node::ErrorType )
        {
            implicitCastLeftChild = Node::String;
            implicitCastRightChild = Node::String;
            returnType = Node::String;
        }
        break;
    }
}

OperationNode::Operation AdditionNode::getOp() const
{
    return OperationNode::Addition;
}

OperationNode::Associativity AdditionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence AdditionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

std::unique_ptr<ValueNode> AdditionNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Double)
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() + value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        //RightChild can only be Integer or Double -> cast anyway
        return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        break;
    case Node::String:
        //RightChild is cast to String anyway
        return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<QString>() + value2->getValue().value<QString>()));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString AdditionNode::printValue() const
{
    return QString("+");
}

QString AdditionNode::printNode() const
{
    QString NodeType = "AdditionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> AdditionNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new AdditionNode(*this));
}


SubtractionNode::SubtractionNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryArithmeticOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer - Integer -> Integer
         * Integer - Double  -> (implicit cast to) Double
         */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Integer;
        }
        else if( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Double:
        /*
         * Double - Double   -> Double
         * Double - Integer  -> (implicit cast to) Double
         */
        if( ( returnTypeRChild == Node::Double ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Bool: // Bool - ??? does not make sense
        break;
    case Node::String: // String - ??? does not make sense
        break;
    }
}

OperationNode::Operation SubtractionNode::getOp() const
{
    return OperationNode::Subtraction;
}

OperationNode::Associativity SubtractionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence SubtractionNode::getPrecedence() const
{
    return OperationNode::AdditivePrec;
}

std::unique_ptr<ValueNode> SubtractionNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Double)
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() - value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString SubtractionNode::printValue() const
{
    return QString("-");
}

QString SubtractionNode::printNode() const
{
    QString NodeType = "SubtractionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> SubtractionNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new SubtractionNode(*this));
}

MultiplicationNode::MultiplicationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryArithmeticOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
          * Integer * Integer -> Integer
          * Integer * Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Integer;
        }
        else if( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Double:
        /*
          * Double * Double   -> Double
          * Double * Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == Node::Double ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Bool: // Bool + ??? does not make sense
        break;
    case Node::String:
        /*
          * String * Integer -> String repeated for the value of the Integer and concatenated
          */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::String;
            implicitCastRightChild = Node::Integer;
            returnType = Node::String;
        }
        break;
    }
}

OperationNode::Operation MultiplicationNode::getOp() const
{
    return OperationNode::Multiplication;
}

OperationNode::Associativity MultiplicationNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence MultiplicationNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

std::unique_ptr<ValueNode> MultiplicationNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Double)
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() * value2->getValue().value<double>()));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() * value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() * value2->getValue().value<double>()));
        break;
    case Node::String:
    {
        QString input(value1->getValue().value<QString>());
        QString output(input);
        int cntr = value2->getValue().value<int>();
        if(cntr--)
        {
            while(cntr)
            {
                output.append(std::move(input));
                cntr--;
            }
        }
        return std::unique_ptr<ValueNode>( new ValueNode( output ));
    }
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString MultiplicationNode::printValue() const
{
    return QString("*");
}

QString MultiplicationNode::printNode() const
{
    QString NodeType = "MultiplicationNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> MultiplicationNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new MultiplicationNode(*this));
}

DivisionNode::DivisionNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryArithmeticOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
          * Integer / Integer -> Integer
          * Integer / Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Integer;
        }
        else if( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Double:
        /*
          * Double / Double   -> Double
          * Double / Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == Node::Double ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Double;
        }
        break;
    case Node::Bool: // Bool / ??? does not make sense
        break;
    case Node::String: // String / ??? does not make sense
        break;
    }
}

OperationNode::Operation DivisionNode::getOp() const
{
    return OperationNode::Division;
}

OperationNode::Associativity DivisionNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence DivisionNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

std::unique_ptr<ValueNode> DivisionNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Double)
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() / value2->getValue().value<int>()));
        }
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString DivisionNode::printValue() const
{
    return QString("/");
}

QString DivisionNode::printNode() const
{
    QString NodeType = "DivisionNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> DivisionNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new DivisionNode(*this));
}

ModuloNode::ModuloNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryArithmeticOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
           * Integer % Integer -> Integer
           */
        if( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // No
        break;
    case Node::Bool: // Bool % ??? does not make sense
        break;
    case Node::String: // String % ??? does not make sense
        break;
    }
}

OperationNode::Operation ModuloNode::getOp() const
{
    return OperationNode::Modulo;
}

OperationNode::Associativity ModuloNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ModuloNode::getPrecedence() const
{
    return OperationNode::MultiplicativePrec;
}

std::unique_ptr<ValueNode> ModuloNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() % value2->getValue().value<int>()));
}

QString ModuloNode::printValue() const
{
    return QString("%");
}

QString ModuloNode::printNode() const
{
    QString NodeType = "ModuloNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> ModuloNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ModuloNode(*this));
}

BinaryLogicalOperationNode::BinaryLogicalOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOperationNode(std::move(leftChild), std::move(rightChild))
{
}

OperationNode::OperationTypes BinaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalANDNode::LogicalANDNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer && Integer -> Bool (true if both are non zero)
         * Integer && Double -> Bool (true if both are non zero)
         * Integer && Bool -> Bool (true if Integer is non zero and Bool is true)
         */
        //FALLTHROUGH
    case Node::Double:
        /*
         * Double && Integer -> Bool (true if both are non zero)
         * Double && Double -> Bool (true if both are non zero)
         * Double && Bool -> Bool (true if Double is non zero and Bool is true)
         */
        //FALLTHROUGH
    case Node::Bool: // Bool % ??? does not make sense
        /*
         * Bool && Integer -> Bool (true if both are non zero)
         * Bool && Double -> Bool (true if both are non zero)
         * Bool && Bool -> Bool (true if both are true)
         */
        if( returnTypeRChild != Node::String )
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::String: // String && ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalANDNode::getOp() const
{
    return OperationNode::LogicalAND;
}

OperationNode::Associativity LogicalANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalANDNode::getPrecedence() const
{
    return OperationNode::LogicalANDPrec;
}

std::unique_ptr<ValueNode> LogicalANDNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() && value2->getValue().value<bool>()));
}

QString LogicalANDNode::printValue() const
{
    return QString("&&");
}

QString LogicalANDNode::printNode() const
{
    QString NodeType = "LogicalANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LogicalANDNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LogicalANDNode(*this));
}

LogicalORNode::LogicalORNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer || Integer -> Bool (true if either is non zero)
         * Integer || Double -> Bool (true if either is non zero)
         * Integer || Bool -> Bool (true if Integer is non zero or Bool is true)
         */
        //FALLTHROUGH
    case Node::Double:
        /*
         * Double || Integer -> Bool (true if either is non zero)
         * Double || Double -> Bool (true if either is non zero)
         * Double || Bool -> Bool (true if Double is non zero or Bool is true)
         */
        //FALLTHROUGH
    case Node::Bool: // Bool % ??? does not make sense
        /*
         * Bool || Integer -> Bool (true if either Bool is true or Integer is non zero)
         * Bool || Double -> Bool (true if either Bool is true or Double is non zero)
         * Bool || Bool -> Bool (true if any of both are true)
         */
        if( returnTypeRChild != Node::String )
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::String: // String || ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalORNode::getOp() const
{
    return OperationNode::LogicalOR;
}

OperationNode::Associativity LogicalORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

std::unique_ptr<ValueNode> LogicalORNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() || value2->getValue().value<bool>()));
}

QString LogicalORNode::printValue() const
{
    return QString("||");
}

QString LogicalORNode::printNode() const
{
    QString NodeType = "LogicalORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LogicalORNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LogicalORNode(*this));
}

LogicalXORNode::LogicalXORNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer ^^ Integer -> Bool (true if only one is non zero)
         * Integer ^^ Double -> Bool (true if only one is non zero)
         * Integer ^^ Bool -> Bool (true if Integer is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case Node::Double:
        /*
         * Double ^^ Integer -> Bool (true if only one is non zero)
         * Double ^^ Double -> Bool (true if only one is non zero)
         * Double ^^ Bool -> Bool (true if Double is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case Node::Bool: // Bool % ??? does not make sense
        /*
         * Bool ^^ Integer -> Bool (true if Bool is true and Integer is zero or vice versa)
         * Bool ^^ Double -> Bool (true if Bool is true and Double is zero or vice versa)
         * Bool ^^ Bool -> Bool (true if only one of both is true)
         */
        if( returnTypeRChild != Node::String )
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::String: // String ^^ ??? does not make sense
        break;
    }
}

OperationNode::Operation LogicalXORNode::getOp() const
{
    return OperationNode::LogicalXOR;
}

OperationNode::Associativity LogicalXORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LogicalXORNode::getPrecedence() const
{
    return OperationNode::LogicalORPrec;
}

std::unique_ptr<ValueNode> LogicalXORNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<bool>() != value2->getValue().value<bool>()));
}

QString LogicalXORNode::printValue() const
{
    return QString("^^");
}

QString LogicalXORNode::printNode() const
{
    QString NodeType = "LogicalXORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LogicalXORNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LogicalXORNode(*this));
}

GreaterNode::GreaterNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer > Integer -> Bool
         * (implicit cast to Double)Integer > Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case Node::Double:
        /*
         * Double > (implicit cast to Double)Integer -> Bool
         * Double > Double -> Bool
         */
        if( ( returnTypeRChild == Node::Integer ) || ( returnTypeRChild == Node::Double ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Bool: // Bool > ??? does not make sense
        break;
    case Node::String: // String > ??? does not make sense
        break;
    }
}

OperationNode::Operation GreaterNode::getOp() const
{
    return OperationNode::Greater;
}

OperationNode::Associativity GreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence GreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

std::unique_ptr<ValueNode> GreaterNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() > value2->getValue().value<double>()) ?  true : false ));
}

QString GreaterNode::printValue() const
{
    return QString(">");
}

QString GreaterNode::printNode() const
{
    QString NodeType = "GreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> GreaterNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new GreaterNode(*this));
}

LowerNode::LowerNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer < Integer -> Bool
         * (implicit cast to Double)Integer < Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case Node::Double:
        /*
         * Double < (implicit cast to Double)Integer -> Bool
         * Double < Double -> Bool
         */
        if( ( returnTypeRChild == Node::Integer ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Bool: // Bool < ??? does not make sense
        break;
    case Node::String: // String < ??? does not make sense
        break;
    }
}

OperationNode::Operation LowerNode::getOp() const
{
    return OperationNode::Lower;
}

OperationNode::Associativity LowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

std::unique_ptr<ValueNode> LowerNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);
    return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() < value2->getValue().value<double>()) ?  true : false ));
}

QString LowerNode::printValue() const
{
    return QString("<");
}

QString LowerNode::printNode() const
{
    QString NodeType = "LowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LowerNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LowerNode(*this));
}

EqualNode::EqualNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer == Integer -> Bool (true if values match)
         * (implicit cast to Double)Integer == Double -> Bool (true if values match)
         * (implicit cast to Bool)Integer == Bool -> Bool
         */
        if(returnTypeRChild == Node::Integer)
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Bool;
        }
        else if( returnTypeRChild == Node::Double)
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        else if( returnTypeRChild == Node::Bool)
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::Double:
        /*
         * Double == (implicit cast to Double)Integer -> Bool (true if values match)
         * Double == Double -> Bool (true if values match)
         * (implicit cast to Bool)Double == Bool -> Bool
         */
        if( ( returnTypeRChild == Node::Double) || (returnTypeRChild == Node::Integer) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        else if( returnTypeRChild == Node::Bool)
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::Bool:
        /*
         * Bool == (implicit cast to Double)Integer -> Bool
         * Bool == (implicit cast to Bool)Double -> Bool
         * Bool == Bool -> Bool
         */
        if( returnTypeRChild != Node::String )
        {
            implicitCastLeftChild = Node::Bool;
            implicitCastRightChild = Node::Bool;
            returnType = Node::Bool;
        }
        break;
    case Node::String:
        // String == String
        if( returnTypeRChild == Node::String )
        {
            implicitCastLeftChild = Node::String;
            implicitCastRightChild = Node::String;
            returnType = Node::Bool;
        }
        break;
    }
}

OperationNode::Operation EqualNode::getOp() const
{
    return OperationNode::Equal;
}

OperationNode::Associativity EqualNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

std::unique_ptr<ValueNode> EqualNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Integer)
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() == value2->getValue().value<int>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Double )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Bool )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Double:
        if((implicitCastRightChild == Node::Double) || ( implicitCastRightChild == Node::Integer ))
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Bool )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Bool:
        return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false ));
        break;
    case Node::String:
        return std::unique_ptr<ValueNode>( new ValueNode((!value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false ));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString EqualNode::printValue() const
{
    return QString("==");
}

QString EqualNode::printNode() const
{
    QString NodeType = "EqualNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> EqualNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new EqualNode(*this));
}

EqualOrGreaterNode::EqualOrGreaterNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer >= Integer -> Bool (true if values match or LChild is Greater)
         * (implicit cast to Double)Integer >= Double -> Bool (true if values match or LChild is Greater)
         */
        if ( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Bool;
        }
        else if ( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Double:
        /*
         * Double >= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Greater)
         * Double >= Double -> Bool (true if values match or LChild is Greater)
         */
        if( ( returnTypeRChild == Node::Double ) || ( returnTypeRChild == Node::Integer ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Bool: // Bool >= ?? does not make sense
        break;
    case Node::String: // String >= ?? does not make sense
        break;
    }
}

OperationNode::Operation EqualOrGreaterNode::getOp() const
{
    return OperationNode::EqualOrGreater;
}

OperationNode::Associativity EqualOrGreaterNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrGreaterNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

std::unique_ptr<ValueNode> EqualOrGreaterNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Integer)
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() >= value2->getValue().value<int>()) ? true : false ));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false ));
        }
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false ));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString EqualOrGreaterNode::printValue() const
{
    return QString(">=");
}

QString EqualOrGreaterNode::printNode() const
{
    QString NodeType = "EqualOrGreaterNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> EqualOrGreaterNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new EqualOrGreaterNode(*this));
}

EqualOrLowerNode::EqualOrLowerNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer <= Integer -> Bool (true if values match or LChild is Lower)
         * (implicit cast to Double)Integer <= Double -> Bool (true if values match or LChild is Lower)
         */
        if ( returnTypeRChild == Node::Integer )
        {
            implicitCastLeftChild = Node::Integer;
            implicitCastRightChild = Node::Integer;
            returnType = Node::Bool;
        }
        else if ( returnTypeRChild == Node::Double )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Double:
        /*
         * Double <= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Lower)
         * Double <= Double -> Bool (true if values match or LChild is Lower)
         */
        if( ( returnTypeRChild == Node::Integer ) || ( returnTypeRChild == Node::Double ) )
        {
            implicitCastLeftChild = Node::Double;
            implicitCastRightChild = Node::Double;
            returnType = Node::Bool;
        }
        break;
    case Node::Bool: // Bool <= ?? does not make sense
        break;
    case Node::String: // String <= ?? does not make sense
        break;
    }
}

OperationNode::Operation EqualOrLowerNode::getOp() const
{
    return OperationNode::EqualOrLower;
}

OperationNode::Associativity EqualOrLowerNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence EqualOrLowerNode::getPrecedence() const
{
    return OperationNode::RelationalPrec;
}

std::unique_ptr<ValueNode> EqualOrLowerNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Integer)
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() <= value2->getValue().value<int>()) ? true : false ));
        }
        else
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false ));
        }
        break;
    case Node::Double:
        return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false ));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString EqualOrLowerNode::printValue() const
{
    return QString("<=");
}

QString EqualOrLowerNode::printNode() const
{
    QString NodeType = "EqualOrLowerNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> EqualOrLowerNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new EqualOrLowerNode(*this));
}

UnequalNode::UnequalNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryLogicalOperationNode(std::move(leftChild), std::move(rightChild))
{
}

OperationNode::Operation UnequalNode::getOp() const
{
    return OperationNode::Unequal;
}

OperationNode::Associativity UnequalNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence UnequalNode::getPrecedence() const
{
    return OperationNode::EqualityPrec;
}

std::unique_ptr<ValueNode> UnequalNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    switch(implicitCastLeftChild)
    {
    case Node::Integer:
        if(implicitCastRightChild == Node::Integer)
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<int>() != value2->getValue().value<int>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Double )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Bool )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Double:
        if((implicitCastRightChild == Node::Double) || ( implicitCastRightChild == Node::Integer ))
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false ));
        }
        else if( implicitCastRightChild == Node::Bool )
        {
            return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        }
        break;
    case Node::Bool:
        return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false ));
        break;
    case Node::String:
        return std::unique_ptr<ValueNode>( new ValueNode((value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false ));
        break;
    default:
        return std::unique_ptr<ValueNode>( new ValueNode());
    }
}

QString UnequalNode::printValue() const
{
    return QString("!=");
}

QString UnequalNode::printNode() const
{
    QString NodeType = "UneqalNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> UnequalNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new UnequalNode(*this));
}

BinaryBitwiseOperationNode::BinaryBitwiseOperationNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryOperationNode(std::move(leftChild), std::move(rightChild))
{
}

OperationNode::OperationTypes BinaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

ANDNode::ANDNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryBitwiseOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer & Integer -> Integer
         */
        if(returnTypeRChild == Node::Integer)
        {
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // Double & ?? does not make sense
        break;
    case Node::Bool: // Bool & ?? does not make sense
        break;
    case Node::String: // String & ?? does not make sense
        break;
    }
}

OperationNode::Operation ANDNode::getOp() const
{
    return OperationNode::AND;
}

OperationNode::Associativity ANDNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ANDNode::getPrecedence() const
{
    return OperationNode::BitwiseANDPrec;
}

std::unique_ptr<ValueNode> ANDNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() & value2->getValue().value<int>()));
}

QString ANDNode::printValue() const
{
    return QString("&");
}

QString ANDNode::printNode() const
{
    QString NodeType = "ANDNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> ANDNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ANDNode(*this));
}

ORNode::ORNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryBitwiseOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer | Integer -> Integer
         */
        if(returnTypeRChild == Node::Integer)
        {
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // Double | ?? does not make sense
        break;
    case Node::Bool: // Bool | ?? does not make sense
        break;
    case Node::String: // String | ?? does not make sense
        break;
    }
}

OperationNode::Operation ORNode::getOp() const
{
    return OperationNode::OR;
}

OperationNode::Associativity ORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence ORNode::getPrecedence() const
{
    return OperationNode::BitwiseORPrec;
}

std::unique_ptr<ValueNode> ORNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() | value2->getValue().value<int>()));
}

QString ORNode::printValue() const
{
    return QString("|");
}

QString ORNode::printNode() const
{
    QString NodeType = "ORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> ORNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new ORNode(*this));
}

XORNode::XORNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryBitwiseOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer ^ Integer -> Integer
         */
        if(returnTypeRChild == Node::Integer)
        {
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // Double ^ ?? does not make sense
        break;
    case Node::Bool: // Bool ^ ?? does not make sense
        break;
    case Node::String: // String ^ ?? does not make sense
        break;
    }
}

OperationNode::Operation XORNode::getOp() const
{
    return OperationNode::XOR;
}

OperationNode::Associativity XORNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence XORNode::getPrecedence() const
{
    return OperationNode::BitwiseXORPrec;
}

std::unique_ptr<ValueNode> XORNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() ^ value2->getValue().value<int>()));
}

QString XORNode::printValue() const
{
    return QString("^");
}

QString XORNode::printNode() const
{
    QString NodeType = "XORNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> XORNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new XORNode(*this));
}

LeftShiftNode::LeftShiftNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryBitwiseOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer << Integer -> Integer
         */
        if(returnTypeRChild == Node::Integer)
        {
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // Double << ?? does not make sense
        break;
    case Node::Bool: // Bool << ?? does not make sense
        break;
    case Node::String: // String << ?? does not make sense
        break;
    }
}

OperationNode::Operation LeftShiftNode::getOp() const
{
    return OperationNode::LeftShift;
}

OperationNode::Associativity LeftShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence LeftShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

std::unique_ptr<ValueNode> LeftShiftNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() << value2->getValue().value<int>()));
}

QString LeftShiftNode::printValue() const
{
    return QString("<<");
}

QString LeftShiftNode::printNode() const
{
    QString NodeType = "LeftShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> LeftShiftNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new LeftShiftNode(*this));
}

RightShiftNode::RightShiftNode(std::unique_ptr<SimpleNode> leftChild, std::unique_ptr<SimpleNode> rightChild) :
    BinaryBitwiseOperationNode(std::move(leftChild), std::move(rightChild))
{
    Node::ValueTypes returnTypeLChild = BinaryOPLeftChild->getReturnType();
    Node::ValueTypes returnTypeRChild = BinaryOPRightChild->getReturnType();

    returnType = Node::ErrorType;

    switch(returnTypeLChild)
    {
    case Node::Integer:
        /*
         * Integer >> Integer -> Integer
         */
        if(returnTypeRChild == Node::Integer)
        {
            returnType = Node::Integer;
        }
        break;
    case Node::Double: // Double >> ?? does not make sense
        break;
    case Node::Bool: // Bool >> ?? does not make sense
        break;
    case Node::String: // String >> ?? does not make sense
        break;
    }
}

OperationNode::Operation RightShiftNode::getOp() const
{
    return OperationNode::RightShift;
}

OperationNode::Associativity RightShiftNode::getAssociativity() const
{
    return OperationNode::LeftToRight;
}

OperationNode::Precedence RightShiftNode::getPrecedence() const
{
    return OperationNode::ShiftPrec;
}

std::unique_ptr<ValueNode> RightShiftNode::DoOperation(QSharedPointer<SimpleStack> StackToUse) const
{
    std::unique_ptr<ValueNode> value1 = BinaryOPLeftChild->visit(StackToUse);
    std::unique_ptr<ValueNode> value2 = BinaryOPRightChild->visit(StackToUse);

    return std::unique_ptr<ValueNode>( new ValueNode(value1->getValue().value<int>() >> value2->getValue().value<int>()));
}

QString RightShiftNode::printValue() const
{
    return QString(">>");
}

QString RightShiftNode::printNode() const
{
    QString NodeType = "RightShiftNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

std::unique_ptr<SimpleNode> RightShiftNode::deepCopy() const
{
    return std::unique_ptr<SimpleNode>(new RightShiftNode(*this));
}
