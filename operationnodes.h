#ifndef OPERATIONNODES_H
#define OPERATIONNODES_H

#include "simpleast.h"

class OperationNode : public SimpleNode
{
public:
    typedef enum _ArityTypes
    {
        Unary,
        Binary,
        Ternary
    }ArityTypes;
    typedef enum _OperationTypes
    {
        Arithmetic,
        Logical,
        Bitwise
    }OperationTypes;
    typedef enum _Operation
    {
        TypeCast,
        Increment,
        Decrement,
        Positive,
        Negative,
        LogicalNegation,
        OnesComplement,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        LogicalAND,
        LogicalOR,
        LogicalXOR,
        Greater,
        Lower,
        Equal,
        EqualOrGreater,
        EqualOrLower,
        Unequal,
        AND,
        OR,
        XOR,
        LeftShift,
        RightShift,
        Conditional
    }Operation;
    typedef enum _Associativity
    {
        LeftToRight,
        RightToLeft
    }Associativity;
    typedef enum _Precedence
    {
        PostfixPrec,
        UnaryPrec,
        MultiplicativePrec,
        AdditivePrec,
        ShiftPrec,
        RelationalPrec,
        EqualityPrec,
        BitwiseANDPrec,
        BitwiseXORPrec,
        BitwiseORPrec,
        LogicalANDPrec,
        LogicalORPrec,
        ConditionalPrec,
        AssignmentPrec,
        CommaPrec
    }Precedence;

    OperationNode();
    virtual ~OperationNode();
    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;
    virtual ArityTypes getArityType() const = 0;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    ValueNodeUniquePtr visit();
protected:
    SimpleNode::ValueTypes returnType;
};

class UnaryOperationNode : public OperationNode
{
public:
    UnaryOperationNode(SimpleNodeUniquePtr rightChild);
    virtual ~UnaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNodeUniquePtr rightChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};



class BinaryOperationNode : public OperationNode
{
public:
    BinaryOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr rightChild);
    virtual ~BinaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNodeUniquePtr leftChild;
    SimpleNodeUniquePtr rightChild;
    SimpleNode::ValueTypes implicitCastLeftChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};


class TernaryOperationNode : public OperationNode
{
public:
    TernaryOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr midChild ,SimpleNodeUniquePtr rightChild);
    virtual ~TernaryOperationNode();
    ArityTypes getArityType() const;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
protected:
    SimpleNodeUniquePtr leftChild;
    SimpleNodeUniquePtr midChild;
    SimpleNodeUniquePtr rightChild;
    SimpleNode::ValueTypes implicitCastLeftChild;
    SimpleNode::ValueTypes implicitCastMidChild;
    SimpleNode::ValueTypes implicitCastRightChild;
};

#endif // OPERATIONNODES_H
