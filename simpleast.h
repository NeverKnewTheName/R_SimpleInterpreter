#ifndef SIMPLEAST_H
#define SIMPLEAST_H

#include <QVariant>
#include <QVector>
#include <QSharedPointer>

//#include "simplesymboltable.h"

class FunctionSymbol;
class VariableSymbol;
class AssignmentNode;

class ValueNode;
typedef QScopedPointer<ValueNode> ValueNodeScopedPtr;

class SimpleNode
{
public:
    typedef enum _NodeType
    {
        Value,
        Data,
        Variable,
        Operation,
        Function,
        FunctionCall,
        Assignment,
        Program,
        EOFNode,
        ERRORNode
    }NodeType;
    typedef enum _ValueTypes
    {
        Integer,
        Double,
        Bool,
        String,
        Void,
        ErrorType
    }ValueTypes;

    SimpleNode();
    virtual ~SimpleNode();

    virtual NodeType getNodeType() const;
    virtual ValueTypes getReturnType() const;

    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual ValueNodeScopedPtr visit();

    static QString getHumanReadableTypeNameToValueType(const ValueTypes type);
    static bool canConvertTypes(const ValueTypes OrigType, const ValueTypes NewType);
protected:
    ValueTypes valueType;
};

typedef QSharedPointer<SimpleNode> SimpleNodePtr;
typedef QScopedPointer<SimpleNode> SimpleNodeScopedPtr;

class ValueNode : public SimpleNode
{
public:

    ValueNode();
    ValueNode(const ValueNode &valueNodeToCopy);
    ValueNode(ValueNode &&valueNodeToMove);
    ValueNode(const int value);
    ValueNode(const double value);
    ValueNode(const bool value);
    ValueNode(QString const &value);
    ~ValueNode();

    NodeType getNodeType() const;
    ValueTypes getReturnType() const;

    ValueNode &operator=(const ValueNode &value);
    ValueNode &operator=(ValueNode &&value);

    const QVariant getValue() const;

    ValueTypes getValueType() const;

    QString printValue() const;
    QString printNode() const;
    ValueNodeScopedPtr visit();

private:
    QVariant value;
};


// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

class EOFNode : public SimpleNode
{
public:
    EOFNode();
    ~EOFNode();

    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    ValueNodeScopedPtr visit();
};
#endif // SIMPLEAST_H
