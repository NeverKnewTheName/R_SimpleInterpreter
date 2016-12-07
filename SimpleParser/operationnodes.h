#ifndef OPERATIONNODES_H
#define OPERATIONNODES_H

#include "simplenode.h"

class ASTNode;

class OperationNode : public NonTerminalNode
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
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    virtual ArityTypes getArityType() const = 0;
    virtual OperationTypes getOpType() const = 0;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation(QSharedPointer<SimpleStack> StackToUse) const = 0;

public:
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

protected:
    Node::ValueTypes returnType;
};

class UnaryOperationNode : public OperationNode
{
public:
    UnaryOperationNode(std::unique_ptr<SimpleNode> RightChild);
    UnaryOperationNode(const UnaryOperationNode &ToCopy);
    virtual ~UnaryOperationNode();
    ArityTypes getArityType() const;

    //virtual uint8_t FlatCompileOPCode(int &curStackOffset) const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;

    virtual ASTNode *VisualizeNode(ASTNode *parentNode) const;
    const std::unique_ptr<SimpleNode> &getUnaryOPRightChild() const;

    Node::ValueTypes getImplicitCastRightChild() const;

protected:
    std::unique_ptr<SimpleNode> UnaryOPRightChild;
    Node::ValueTypes implicitCastRightChild;
};

class BinaryOperationNode : public OperationNode
{
public:
    BinaryOperationNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> RightChild);
    BinaryOperationNode(const BinaryOperationNode &ToCopy);
    virtual ~BinaryOperationNode();
    ArityTypes getArityType() const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;

    virtual ASTNode *VisualizeNode(ASTNode *parentNode) const;

    const std::unique_ptr<SimpleNode> &getBinaryOPLeftChild() const;

    const std::unique_ptr<SimpleNode> &getBinaryOPRightChild() const;

    Node::ValueTypes getImplicitCastLeftChild() const;

    Node::ValueTypes getImplicitCastRightChild() const;

protected:
    std::unique_ptr<SimpleNode> BinaryOPLeftChild;
    std::unique_ptr<SimpleNode> BinaryOPRightChild;
    Node::ValueTypes implicitCastLeftChild;
    Node::ValueTypes implicitCastRightChild;
};


class TernaryOperationNode : public OperationNode
{
public:
    TernaryOperationNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> midChild ,std::unique_ptr<SimpleNode> rightChild);
    TernaryOperationNode(const TernaryOperationNode &ToCopy);
    virtual ~TernaryOperationNode();
    ArityTypes getArityType() const;
    //virtual uint8_t FlatCompileOPCode(int &curStackOffset) const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;

    virtual ASTNode *VisualizeNode(ASTNode *parentNode) const;

    const std::unique_ptr<SimpleNode> &getTernaryOPLeftChild() const;

    const std::unique_ptr<SimpleNode> &getTernaryOPMidChild() const;

    const std::unique_ptr<SimpleNode> &getTernaryOPRightChild() const;

    Node::ValueTypes getImplicitCastLeftChild() const;

    Node::ValueTypes getImplicitCastMidChild() const;

    Node::ValueTypes getImplicitCastRightChild() const;

protected:
    std::unique_ptr<SimpleNode> TernaryOPLeftChild;
    std::unique_ptr<SimpleNode> TernaryOPMidChild;
    std::unique_ptr<SimpleNode> TernaryOPRightChild;
    Node::ValueTypes implicitCastLeftChild;
    Node::ValueTypes implicitCastMidChild;
    Node::ValueTypes implicitCastRightChild;
};

#endif // OPERATIONNODES_H
