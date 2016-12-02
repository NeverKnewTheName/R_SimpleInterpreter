#ifndef SIMPLENODE_H
#define SIMPLENODE_H

#include <QVariant>
#include <QSharedPointer>
#include <memory>

class ValueNode;
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

//namespace R_CCompiler {
//typedef enum _Instruction {
//    StackPush,
//    StackPop,
//    Load,
//    Store,
//    Branch
//}Instruction;
//typedef enum _Data {
//    Address,
//    Function,
//    Value
//}Data;
//typedef enum _Register {
//    R0,//!< Work register 0
//    R1,//!< Work register 1
//    R2,//!< Work register 2
//    R3,//!< Work register 3
//    PC,//!< Program counter
//    LR,//!< Link register
//    SP,//!< Stack pointer
//    XR//!< Special purpose register
//}Register;
//typedef enum _XRFlags
//{
//    Negative,
//    Zero,
//    Carry,
//    Overflow
//}XRFlags;
//}

class SimpleNode
{
public:

    SimpleNode(Node::ValueTypes valueType = Node::ErrorType);
    virtual ~SimpleNode();

    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;

    virtual QString printValue() const;
    virtual QString printNode() const;

    virtual std::unique_ptr<SimpleNode> deepCopy() const = 0;

    virtual std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

    virtual std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode>>> FlatAST, QSharedPointer<SimpleStack> StackToUse) const = 0;

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

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;    

    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, QSharedPointer<SimpleStack> StackToUse) const;

    std::unique_ptr<SimpleNode> deepCopy() const;
};
#endif // SIMPLENODE_H
