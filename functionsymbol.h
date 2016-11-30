#ifndef FUNCTIONSYMBOL_H
#define FUNCTIONSYMBOL_H

#include <QString>
#include "vector"
#include "symbol.h"

#include "simplenode.h"
#include "valuenode.h"
#include "variablesymbol.h"

class SymbolTable;

class FunctionSymbol : public Symbol
{
public:
    FunctionSymbol(const QString &identifier,
            std::vector<QSharedPointer<VariableSymbol> > &&functionParameters,
            Node::ValueTypes ReturnType = Node::Void
            );
    ~FunctionSymbol();

    void addFunctionExpressions(std::vector<std::unique_ptr<SimpleNode>> &FuncExpressions);
    void addFunctionReturnStatement(std::unique_ptr<SimpleNode> returnNode);

    std::unique_ptr<ValueNode> CallFunction(
            const std::vector<std::unique_ptr<SimpleNode> > &FunctionArguments,
            QSharedPointer<SymbolTable> CurrentSymbolTable
            );

    bool checkFunctionArguments(const QVector<std::unique_ptr<SimpleNode>> &FunctionArguments) const;

    QSharedPointer<SymbolTable> getFunctionSymbolTable() const;
    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    Node::ValueTypes getReturnType() const;

private:
    QSharedPointer<SymbolTable> FunctionSymbolTable;
    Node::ValueTypes ReturnType;
    std::vector<QSharedPointer<VariableSymbol>> FunctionParameters;
    std::vector<std::unique_ptr<SimpleNode>> FunctionExpressions;
    std::unique_ptr<SimpleNode> FunctionReturnNode;
};

#endif // FUNCTIONSYMBOL_H
