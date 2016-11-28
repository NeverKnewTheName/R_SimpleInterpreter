#ifndef TERNARYOPERATIONNODES_H
#define TERNARYOPERATIONNODES_H

#include "operationnodes.h"

class TernaryArithmeticOperationNode : public TernaryOperationNode
{
public:
    TernaryArithmeticOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild ,const SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TernaryLogicalOperationNode : public TernaryOperationNode
{
public:
    TernaryLogicalOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild ,const SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ConditionalNode : public TernaryLogicalOperationNode
{
public:
    ConditionalNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild ,const SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    const ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class TernaryBitwiseOperationNode : public TernaryOperationNode
{
public:
    TernaryBitwiseOperationNode(const SimpleNodeScopedPtr leftChild, const SimpleNodeScopedPtr midChild ,const SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual const ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

#endif // TERNARYOPERATIONNODES_H
