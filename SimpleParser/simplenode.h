#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include <QVariant>
#include <QSharedPointer>
#include <memory>


class ASTNode;
class ValueNode;
class SimpleSymbolTable;
class SimpleStack;

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
    Control,
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

    virtual ASTNode *VisualizeNode(ASTNode *parentNode = nullptr) const;
    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatAST, int &maxStackSize, int &CurrentPosition) const;

    static QString getHumanReadableTypeNameToValueType(const Node::ValueTypes type);
    static bool canConvertTypes(const Node::ValueTypes OrigType, const Node::ValueTypes NewType);

protected:
    Node::ValueTypes valueType;
};
// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

class ScopedNode : public SimpleNode
{
    ScopedNode(QSharedPointer<SimpleSymbolTable> ScopedSymbolTable);
    virtual ~ScopedNode();

    // SimpleNode interface
public:
    virtual Node::NodeType getNodeType() const = 0;
    virtual Node::ValueTypes getReturnType() const = 0;
    virtual QString printValue() const = 0;
    virtual QString printNode() const = 0;
    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;
    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const = 0;

private:
    QSharedPointer<SimpleSymbolTable> ScopedSymbolTable;
};

// // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
class EOFNode : public SimpleNode
{
public:
    EOFNode();
    ~EOFNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;    

    std::unique_ptr<SimpleNode> deepCopy() const;
};
#endif // SIMPLENODE_H
