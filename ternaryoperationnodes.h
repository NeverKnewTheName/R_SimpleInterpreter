#ifndef TERNARYOPERATIONNODES_H
#define TERNARYOPERATIONNODES_H

#include "operationnodes.h"

class TernaryArithmeticOperationNode : public TernaryOperationNode
{
public:
    TernaryArithmeticOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr midChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class TernaryLogicalOperationNode : public TernaryOperationNode
{
public:
    TernaryLogicalOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr midChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

class ConditionalNode : public TernaryLogicalOperationNode
{
public:
    ConditionalNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr midChild, SimpleNodeUniquePtr rightChild);
    Operation getOp() const;
    Associativity getAssociativity() const;
    Precedence getPrecedence() const;

    ValueNodeUniquePtr DoOperation();

    QString printValue() const;
    QString printNode() const;
};

class TernaryBitwiseOperationNode : public TernaryOperationNode
{
public:
    TernaryBitwiseOperationNode(SimpleNodeUniquePtr leftChild, SimpleNodeUniquePtr midChild, SimpleNodeUniquePtr rightChild);
    OperationTypes getOpType() const;
    virtual Operation getOp() const = 0;
    virtual Associativity getAssociativity() const = 0;
    virtual Precedence getPrecedence() const = 0;

    virtual ValueNodeUniquePtr DoOperation() = 0;

    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
};

#endif // TERNARYOPERATIONNODES_H
