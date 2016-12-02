#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include <QString>
#include <QSharedPointer>
#include "variablesymbol.h"

class VariableNode : public SimpleNode
{
public:
    VariableNode(QSharedPointer<ValueSymbol> relatedVariableSymbol);
    ~VariableNode();

    QString getVariableName() const;
    QSharedPointer<ValueSymbol> getRelatedVariableSymbol() const;

    // SimpleNode interface
public:

    virtual Node::NodeType getNodeType() const;
    virtual Node::ValueTypes getReturnType() const;
    QString printValue() const;
    virtual QString printNode() const;

    std::unique_ptr<ValueNode> visit(QSharedPointer<SimpleStack> StackToUse) const;
    std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatCompile(std::unique_ptr<std::vector<std::unique_ptr<SimpleNode> > > FlatAST, QSharedPointer<SimpleStack> StackToUse) const;

private:
    QSharedPointer<ValueSymbol> RelatedVariableSymbol;    
};

#endif // VARIABLENODE_H
