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
    FunctionCallNode(const FunctionCallNode &ToCopy);
    virtual ~FunctionCallNode();

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    ASTNode *VisualizeNode(ASTNode *parentNode) const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    uint8_t FlatCompileOPCode(int &curStackOffset) const;

    std::unique_ptr<SimpleNode> deepCopy() const;
    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, int &maxStackSize, int &CurrentPosition) const;
private:
    QString FunctionName;
    std::vector<std::unique_ptr<SimpleNode>> FuncArgs;
    QSharedPointer<FunctionSymbol> RelatedSymbol;
    QSharedPointer<SimpleSymbolTable> CurrentSymbolTable;
    Node::ValueTypes returnType;
};

#endif // FUNCTIONCALLNODE_H
