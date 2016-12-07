#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include <QString>
#include <QSharedPointer>
#include "variablesymbol.h"

class VariableNode : public TerminalNode
{
public:
    VariableNode(QSharedPointer<ValueSymbol> relatedVariableSymbol);
    VariableNode(const VariableNode &ToCopy);
    ~VariableNode();

    QString getVariableName() const;
    QSharedPointer<ValueSymbol> getRelatedVariableSymbol() const;

    // SimpleNode interface
public:
    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;
    virtual ASTNode *VisualizeNode(ASTNode *parentNode) const;
    virtual QString printValue() const;
    virtual QString printNode() const;

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    std::unique_ptr<SimpleNode> deepCopy() const;

private:
    QSharedPointer<ValueSymbol> RelatedVariableSymbol;    

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};

#endif // VARIABLENODE_H
