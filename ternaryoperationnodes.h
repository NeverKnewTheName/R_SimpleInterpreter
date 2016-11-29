#ifndef TERNARYOPERATIONNODES_H
#define TERNARYOPERATIONNODES_H

#include "operationnodes.h"

class TernaryArithmeticOperationNode : public TernaryOperationNode
{
public:
    TernaryArithmeticOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr midChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TernaryLogicalOperationNode : public TernaryOperationNode
{
public:
    TernaryLogicalOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr midChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ConditionalNode : public TernaryLogicalOperationNode
{
public:
    ConditionalNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr midChild, SimpleNodeScopedPtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeScopedPtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class TernaryBitwiseOperationNode : public TernaryOperationNode
{
public:
    TernaryBitwiseOperationNode(SimpleNodeScopedPtr leftChild, SimpleNodeScopedPtr midChild, SimpleNodeScopedPtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeScopedPtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

#endif // TERNARYOPERATIONNODES_H
