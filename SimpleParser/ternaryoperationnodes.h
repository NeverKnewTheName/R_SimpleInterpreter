#ifndef TERNARYOPERATIONNODES_H
#define TERNARYOPERATIONNODES_H

#include "operationnodes.h"

class TernaryArithmeticOperationNode : public TernaryOperationNode
{
public:
    TernaryArithmeticOperationNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation(QSharedPointer<SimpleStack> StackToUse) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const = 0;
};

class TernaryLogicalOperationNode : public TernaryOperationNode
{
public:
    TernaryLogicalOperationNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation(QSharedPointer<SimpleStack> StackToUse) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const = 0;
};

class ConditionalNode : public TernaryLogicalOperationNode
{
public:
    ConditionalNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    std::unique_ptr<ValueNode> DoOperation(QSharedPointer<SimpleStack> StackToUse) const;

    QString printValue() const;
    QString printNode() const;

    // SimpleNode interface
public:
    std::unique_ptr<SimpleNode> deepCopy() const;

    // SimpleNode interface
public:
    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const;
};

class TernaryBitwiseOperationNode : public TernaryOperationNode
{
public:
    TernaryBitwiseOperationNode(std::unique_ptr<SimpleNode> LeftChild, std::unique_ptr<SimpleNode> midChild, std::unique_ptr<SimpleNode> rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual std::unique_ptr<ValueNode> DoOperation(QSharedPointer<SimpleStack> StackToUse) const = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    // SimpleNode interface
public:
    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize) const = 0;
};

#endif // TERNARYOPERATIONNODES_H
