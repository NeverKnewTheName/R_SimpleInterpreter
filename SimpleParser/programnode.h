#ifndef PROGRAMNODE_H
#define PROGRAMNODE_H

#include <QString>
#include <vector>

#include "simplenode.h"

class SimpleSymbolTable;
class VariableSymbol;
class FunctionSymbol;

class ProgramNode : public NonTerminalNode
{
public:
    ProgramNode(
            const QString &ProgramName,
            QSharedPointer<SimpleSymbolTable> SymbolTableToUse
            );
    ProgramNode(const ProgramNode &ToCopy);
    ~ProgramNode();

    void addExpression(std::unique_ptr<SimpleNode> &newExpression);
    void addFunctionDefinition(QSharedPointer<FunctionSymbol> newFunction);
    void addVariableDefinition(QSharedPointer<VariableSymbol> newVariable);
    void addReturnStatement(std::unique_ptr<SimpleNode> ReturnStatement);

    QSharedPointer<SimpleSymbolTable> getProgramSymbolTable() const;

    const std::vector<std::unique_ptr<SimpleNode> > &getProgramExpressions() const;
    const std::unique_ptr<SimpleNode> &getProgramReturnStatement() const;

    // SimpleNode interface
public:
    Node::NodeType getNodeType() const;
    Node::ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    std::unique_ptr<SimpleNode> deepCopy() const;

private:
    QString ProgramName;
    Node::ValueTypes type;
    std::vector<std::unique_ptr<SimpleNode>> ProgramExpressions;
    std::unique_ptr<SimpleNode> ProgramReturnStatement;
    QSharedPointer<SimpleSymbolTable> ParentSymbolTable;
    QSharedPointer<SimpleSymbolTable> ProgramSymbolTable;

    // SimpleNodeVisitable interface
public:
    void accept(SimpleNodeVisitor *visitor) const;
};


#endif // PROGRAMNODE_H
