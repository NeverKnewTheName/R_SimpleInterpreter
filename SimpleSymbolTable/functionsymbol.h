#ifndef FUNCTIONSYMBOL_H
#define FUNCTIONSYMBOL_H

#include <QString>
#include "vector"
#include "simplesymbol.h"

#include "simplenode.h"
#include "valuenode.h"
#include "variablesymbol.h"

class SimpleSymbolTable;

class FunctionSymbol : public SimpleSymbol
{
public:
    FunctionSymbol(
            const QString &identifier,
            QSharedPointer<SimpleSymbolTable> SymbolTableForFunction,
            std::vector<QSharedPointer<VariableSymbol> > &&functionParameters,
            Node::ValueTypes ReturnType = Node::Void
            );
    ~FunctionSymbol();

    void addFunctionExpressions(std::vector<std::unique_ptr<SimpleNode>> &FuncExpressions);
    void addFunctionReturnStatement(std::unique_ptr<SimpleNode> returnNode);
//    void addVariableDefinition(QSharedPointer<VariableSymbol> newVariable);

    std::unique_ptr<ValueNode> CallFunction(
            const std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments,
            QSharedPointer<SimpleStack> StackToUse
            );

    bool checkFunctionArguments(const QVector<std::unique_ptr<SimpleNode>> &FunctionArguments) const;

    QSharedPointer<SimpleSymbolTable> getFunctionSymbolTable() const;

    bool BuildFunctionStack(QSharedPointer<SimpleStack> StackToUse) const;
    bool DestroyFunctionStack(QSharedPointer<SimpleStack> StackToUse) const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    Node::ValueTypes getReturnType() const;

    const std::vector<std::unique_ptr<SimpleNode> > &getFunctionExpressions() const;

    const std::unique_ptr<SimpleNode> &getFunctionReturnNode() const;

    const std::vector<QSharedPointer<VariableSymbol> > &getFunctionParameters() const;

private:
    QSharedPointer<SimpleSymbolTable> FunctionSymbolTable;
    Node::ValueTypes ReturnType;
    std::vector<QSharedPointer<VariableSymbol>> FunctionParameters;
    std::vector<std::unique_ptr<SimpleNode>> FunctionExpressions;
    std::unique_ptr<SimpleNode> FunctionReturnNode;
};

#endif // FUNCTIONSYMBOL_H
