#ifndef FUNCTIONCALLNODE_H
#define FUNCTIONCALLNODE_H

#include <vector>
#include "simplenode.h"

class SimpleSymbolTable;
class FunctionSymbol;

class FunctionCallNode : public SimpleNode
{
public:
    FunctionCallNode(
            const QString &FunctionName,
            QSharedPointer<SimpleSymbolTable> CurSymblTable,
            std::vector<std::unique_ptr<SimpleNode>> &FunctionArguments
            );
    virtual ~FunctionCallNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;

private:
    QString FunctionName;
    std::vector<std::unique_ptr<SimpleNode>> FuncArgs;
    QSharedPointer<FunctionSymbol> RelatedSymbol;
    QSharedPointer<SimpleSymbolTable> CurrentSymbolTable;
    Node::ValueTypes returnType;
};

#endif // FUNCTIONCALLNODE_H
