#include "binaryoperationnodes.h"

BinaryArithmeticOperationNode::BinaryArithmeticOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryArithmeticOperationNode::getOpType() const
{
    return OperationNode::Arithmetic;
}

AdditionNode::AdditionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer + Integer -> Integer
         * (implicit cast to) Integer + Double  -> Double
         */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = ValueNode::Integer;
            implicitCastRightChild = ValueNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = ValueNode::Double;
            implicitCastRightChild = ValueNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
         * Double + Double   -> Double
         * Double + (implicit cast to) Integer  -> Double
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = ValueNode::Double;
            implicitCastRightChild = ValueNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool + ??? does not make sense
        break;
    case ValueNode::String:
        /*
         * String + String  -> Concatenation
         * String + Integer/Double -> String + String(Integer/Double)
         * String + Bool -> String + Bool ? String("true") : String("false")
         */
        if( returnTypeRChild != ValueNode::ErrorType )
        {
            implicitCastLeftChild = ValueNode::String;
            implicitCastRightChild = ValueNode::String;
            returnType = ValueNode::String;
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

ValueNodeUniquePtr AdditionNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == ValueNode::Double)
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() + value2->getValue().value<int>()));
        }
        break;
    case ValueNode::Double:
        //RightChild can only be Integer or Double -> cast anyway
        return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() + value2->getValue().value<double>()));
        break;
    case ValueNode::String:
        //RightChild is cast to String anyway
        return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<QString>() + value2->getValue().value<QString>()));
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

SubtractionNode::SubtractionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer - Integer -> Integer
         * Integer - Double  -> (implicit cast to) Double
         */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
         * Double - Double   -> Double
         * Double - Integer  -> (implicit cast to) Double
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool - ??? does not make sense
        break;
    case ValueNode::String: // String - ??? does not make sense
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

ValueNodeUniquePtr SubtractionNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() - value2->getValue().value<int>()));
        }
        break;
    case ValueNode::Double:
        return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() - value2->getValue().value<double>()));
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

MultiplicationNode::MultiplicationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
          * Integer * Integer -> Integer
          * Integer * Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        else if( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Double:
        /*
          * Double * Double   -> Double
          * Double * Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case ValueNode::Bool: // Bool + ??? does not make sense
        break;
    case ValueNode::String:
        /*
          * String * Integer -> String repeated for the value of the Integer and concatenated
          */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::String;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::String;
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

ValueNodeUniquePtr MultiplicationNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() * value2->getValue().value<double>()));
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() * value2->getValue().value<int>()));
        }
        break;
    case ValueNode::Double:
        return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() * value2->getValue().value<double>()));
        break;
    case SimpleNode::String:
    {
        QString input(value1->getValue().value<QString>();
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
        return ValueNodeUniquePtr( new ValueNode( output ));
    }
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

DivisionNode::DivisionNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case SimpleNode::Integer:
        /*
          * Integer / Integer -> Integer
          * Integer / Double  -> (implicit cast to) Double
          */
        if( returnTypeRChild == SimpleNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = SimpleNode::Integer;
        }
        else if( returnTypeRChild == SimpleNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Double;
        }
        break;
    case SimpleNode::Double:
        /*
          * Double / Double   -> Double
          * Double / Integer  -> (implicit cast to) Double
          */
        if( ( returnTypeRChild == SimpleNode::Double ) || ( returnTypeRChild == SimpleNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = SimpleNode::Double;
        }
        break;
    case SimpleNode::Bool: // Bool / ??? does not make sense
        break;
    case SimpleNode::String: // String / ??? does not make sense
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

ValueNodeUniquePtr DivisionNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Double)
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() / value2->getValue().value<int>()));
        }
        break;
    case ValueNode::Double:
        return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<double>() / value2->getValue().value<double>()));
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

ModuloNode::ModuloNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryArithmeticOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
           * Integer % Integer -> Integer
           */
        if( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // No
        break;
    case ValueNode::Bool: // Bool % ??? does not make sense
        break;
    case ValueNode::String: // String % ??? does not make sense
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

ValueNodeUniquePtr ModuloNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() % value2->getValue().value<int>()));
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

BinaryLogicalOperationNode::BinaryLogicalOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryLogicalOperationNode::getOpType() const
{
    return OperationNode::Logical;
}

LogicalANDNode::LogicalANDNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer && Integer -> Bool (true if both are non zero)
         * Integer && Double -> Bool (true if both are non zero)
         * Integer && Bool -> Bool (true if Integer is non zero and Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double && Integer -> Bool (true if both are non zero)
         * Double && Double -> Bool (true if both are non zero)
         * Double && Bool -> Bool (true if Double is non zero and Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool && Integer -> Bool (true if both are non zero)
         * Bool && Double -> Bool (true if both are non zero)
         * Bool && Bool -> Bool (true if both are true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String && ??? does not make sense
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

ValueNodeUniquePtr LogicalANDNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<bool>() && value2->getValue().value<bool>()));
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

LogicalORNode::LogicalORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer || Integer -> Bool (true if either is non zero)
         * Integer || Double -> Bool (true if either is non zero)
         * Integer || Bool -> Bool (true if Integer is non zero or Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double || Integer -> Bool (true if either is non zero)
         * Double || Double -> Bool (true if either is non zero)
         * Double || Bool -> Bool (true if Double is non zero or Bool is true)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool || Integer -> Bool (true if either Bool is true or Integer is non zero)
         * Bool || Double -> Bool (true if either Bool is true or Double is non zero)
         * Bool || Bool -> Bool (true if any of both are true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String || ??? does not make sense
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

ValueNodeUniquePtr LogicalORNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<bool>() || value2->getValue().value<bool>()));
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

LogicalXORNode::LogicalXORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer ^^ Integer -> Bool (true if only one is non zero)
         * Integer ^^ Double -> Bool (true if only one is non zero)
         * Integer ^^ Bool -> Bool (true if Integer is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double ^^ Integer -> Bool (true if only one is non zero)
         * Double ^^ Double -> Bool (true if only one is non zero)
         * Double ^^ Bool -> Bool (true if Double is non zero or Bool is false and vice versa)
         */
        //FALLTHROUGH
    case ValueNode::Bool: // Bool % ??? does not make sense
        /*
         * Bool ^^ Integer -> Bool (true if Bool is true and Integer is zero or vice versa)
         * Bool ^^ Double -> Bool (true if Bool is true and Double is zero or vice versa)
         * Bool ^^ Bool -> Bool (true if only one of both is true)
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String: // String ^^ ??? does not make sense
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

ValueNodeUniquePtr LogicalXORNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<bool>() != value2->getValue().value<bool>()));
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

GreaterNode::GreaterNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer > Integer -> Bool
         * (implicit cast to Double)Integer > Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double > (implicit cast to Double)Integer -> Bool
         * Double > Double -> Bool
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Double ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool > ??? does not make sense
        break;
    case ValueNode::String: // String > ??? does not make sense
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

ValueNodeUniquePtr GreaterNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() > value2->getValue().value<double>()) ?  true : false ));
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

LowerNode::LowerNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer < Integer -> Bool
         * (implicit cast to Double)Integer < Double -> Bool
         */
        //ToDO IMPLICIT CASTS
        //FALLTHROUGH
    case ValueNode::Double:
        /*
         * Double < (implicit cast to Double)Integer -> Bool
         * Double < Double -> Bool
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool < ??? does not make sense
        break;
    case ValueNode::String: // String < ??? does not make sense
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

ValueNodeUniquePtr LowerNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());
    return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() < value2->getValue().value<double>()) ?  true : false ));
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

EqualNode::EqualNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer == Integer -> Bool (true if values match)
         * (implicit cast to Double)Integer == Double -> Bool (true if values match)
         * (implicit cast to Bool)Integer == Bool -> Bool
         */
        if(returnTypeRChild == SimpleNode::Integer)
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Double)
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Bool)
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double == (implicit cast to Double)Integer -> Bool (true if values match)
         * Double == Double -> Bool (true if values match)
         * (implicit cast to Bool)Double == Bool -> Bool
         */
        if( ( returnTypeRChild == SimpleNode::Double) || (returnTypeRChild == SimpleNode::Integer) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        else if( returnTypeRChild == SimpleNode::Bool)
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool:
        /*
         * Bool == (implicit cast to Double)Integer -> Bool
         * Bool == (implicit cast to Bool)Double -> Bool
         * Bool == Bool -> Bool
         */
        if( returnTypeRChild != ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::Bool;
            implicitCastRightChild = SimpleNode::Bool;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::String:
        // String == String
        if( returnTypeRChild == ValueNode::String )
        {
            implicitCastLeftChild = SimpleNode::String;
            implicitCastRightChild = SimpleNode::String;
            returnType = ValueNode::Bool;
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

ValueNodeUniquePtr EqualNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<int>() == value2->getValue().value<int>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Double )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        if((implicitCastRightChild == SimpleNode::Double) || ( implicitCastRightChild == SimpleNode::Integer ))
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() == value2->getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false );
        }
        break;
    case SimpleNode::Bool:
        return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() == value2->getValue().value<bool>()) ? true : false );
        break;
    case ValueNode::String:
        return ValueNodeUniquePtr( new ValueNode((!value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false );
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

EqualOrGreaterNode::EqualOrGreaterNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer >= Integer -> Bool (true if values match or LChild is Greater)
         * (implicit cast to Double)Integer >= Double -> Bool (true if values match or LChild is Greater)
         */
        if ( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if ( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double >= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Greater)
         * Double >= Double -> Bool (true if values match or LChild is Greater)
         */
        if( ( returnTypeRChild == ValueNode::Double ) || ( returnTypeRChild == ValueNode::Integer ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool >= ?? does not make sense
        break;
    case ValueNode::String: // String >= ?? does not make sense
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

ValueNodeUniquePtr EqualOrGreaterNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<int>() >= value2->getValue().value<int>()) ? true : false );
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() >= value2->getValue().value<double>()) ? true : false );
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode();
    }
    return Result;
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

EqualOrLowerNode::EqualOrLowerNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer <= Integer -> Bool (true if values match or LChild is Lower)
         * (implicit cast to Double)Integer <= Double -> Bool (true if values match or LChild is Lower)
         */
        if ( returnTypeRChild == ValueNode::Integer )
        {
            implicitCastLeftChild = SimpleNode::Integer;
            implicitCastRightChild = SimpleNode::Integer;
            returnType = ValueNode::Bool;
        }
        else if ( returnTypeRChild == ValueNode::Double )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Double:
        /*
         * Double <= (implicit cast to Double)Integer -> Bool (true if values match or LChild is Lower)
         * Double <= Double -> Bool (true if values match or LChild is Lower)
         */
        if( ( returnTypeRChild == ValueNode::Integer ) || ( returnTypeRChild == ValueNode::Double ) )
        {
            implicitCastLeftChild = SimpleNode::Double;
            implicitCastRightChild = SimpleNode::Double;
            returnType = ValueNode::Bool;
        }
        break;
    case ValueNode::Bool: // Bool <= ?? does not make sense
        break;
    case ValueNode::String: // String <= ?? does not make sense
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

ValueNodeUniquePtr EqualOrLowerNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<int>() <= value2->getValue().value<int>()) ? true : false );
        }
        else
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() <= value2->getValue().value<double>()) ? true : false );
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

UnequalNode::UnequalNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryLogicalOperationNode(leftChild, rightChild)
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

ValueNodeUniquePtr UnequalNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    switch(implicitCastLeftChild)
    {
    case ValueNode::Integer:
        if(implicitCastRightChild == SimpleNode::Integer)
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<int>() != value2->getValue().value<int>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Double )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false );
        }
        break;
    case ValueNode::Double:
        if((implicitCastRightChild == SimpleNode::Double) || ( implicitCastRightChild == SimpleNode::Integer ))
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<double>() != value2->getValue().value<double>()) ? true : false );
        }
        else if( implicitCastRightChild == SimpleNode::Bool )
        {
            return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false );
        }
        break;
    case SimpleNode::Bool:
        return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<bool>() != value2->getValue().value<bool>()) ? true : false );
        break;
    case ValueNode::String:
        return ValueNodeUniquePtr( new ValueNode((value1->getValue().value<QString>().compare(value2->getValue().value<QString>())) ? true : false );
        break;
    default:
        return ValueNodeUniquePtr( new ValueNode());
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

BinaryBitwiseOperationNode::BinaryBitwiseOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryOperationNode(leftChild, rightChild)
{
}

OperationNode::OperationTypes BinaryBitwiseOperationNode::getOpType() const
{
    return OperationNode::Bitwise;
}

ANDNode::ANDNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer & Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double & ?? does not make sense
        break;
    case ValueNode::Bool: // Bool & ?? does not make sense
        break;
    case ValueNode::String: // String & ?? does not make sense
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

ValueNodeUniquePtr ANDNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() & value2->getValue().value<int>()));
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

ORNode::ORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer | Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double | ?? does not make sense
        break;
    case ValueNode::Bool: // Bool | ?? does not make sense
        break;
    case ValueNode::String: // String | ?? does not make sense
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

ValueNodeUniquePtr ORNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() | value2->getValue().value<int>()));
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

XORNode::XORNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer ^ Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double ^ ?? does not make sense
        break;
    case ValueNode::Bool: // Bool ^ ?? does not make sense
        break;
    case ValueNode::String: // String ^ ?? does not make sense
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

ValueNodeUniquePtr XORNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() ^ value2->getValue().value<int>()));
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

LeftShiftNode::LeftShiftNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer << Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double << ?? does not make sense
        break;
    case ValueNode::Bool: // Bool << ?? does not make sense
        break;
    case ValueNode::String: // String << ?? does not make sense
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

ValueNodeUniquePtr LeftShiftNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() << value2->getValue().value<int>()));
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

RightShiftNode::RightShiftNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild) :
    BinaryBitwiseOperationNode(leftChild, rightChild)
{
    SimpleNode::ValueTypes returnTypeLChild = leftChild->getReturnType();
    SimpleNode::ValueTypes returnTypeRChild = rightChild->getReturnType();

    returnType = ValueNode::ErrorType;

    switch(returnTypeLChild)
    {
    case ValueNode::Integer:
        /*
         * Integer >> Integer -> Integer
         */
        if(returnTypeRChild == ValueNode::Integer)
        {
            returnType = ValueNode::Integer;
        }
        break;
    case ValueNode::Double: // Double >> ?? does not make sense
        break;
    case ValueNode::Bool: // Bool >> ?? does not make sense
        break;
    case ValueNode::String: // String >> ?? does not make sense
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

ValueNodeUniquePtr RightShiftNode::DoOperation()
{
    ValueNodeUniquePtr value1(leftChild->visit().take());
    ValueNodeUniquePtr value2(richtChild->visit().take());

    return ValueNodeUniquePtr( new ValueNode(value1->getValue().value<int>() >> value2->getValue().value<int>()));
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
