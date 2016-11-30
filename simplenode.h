#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include <QVariant>
#include <QSharedPointer>
#include <memory>

class ValueNode;

namespace Node {
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
}

class SimpleNode
{
public:

    SimpleNode(Node::ValueTypes valueType = Node::ErrorType);
    virtual ~SimpleNode();

    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;

    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual std::unique_ptr<ValueNode> visit() const;

    static QString getHumanReadableTypeNameToValueType(const Node::ValueTypes type);
    static bool canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType);

protected:
    Node::ValueTypes valueType;
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

class EOFNode : public SimpleNode
{
public:
    EOFNode();
    ~EOFNode();

    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    std::unique_ptr<ValueNode> visit() const;
};
#endif // SIMPLENODE_H
