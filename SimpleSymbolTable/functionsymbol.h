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

    bool checkFunctionArguments(const QVector<std::unique_ptr<SimpleNode>> &FunctionArguments) const;

    QSharedPointer<SimpleSymbolTable> getFunctionSymbolTable() const;

    // SymbolTableEntry interface
public:
    SymbolTableEntryType getType() const;
    QString PrintToSymbolToString() const;
    QString PrintSymbolType() const;

    // Symbol interface
public:
    Node::ValueTypes getReturnType() const;

    const std::vector<QSharedPointer<VariableSymbol> > &getFunctionParameters() const;

    const std::unique_ptr<SimpleNode> &getFunctionStatement() const;
    void setFunctionStatement(std::unique_ptr<SimpleNode> value);

private:
    QSharedPointer<SimpleSymbolTable> FunctionSymbolTable;
    Node::ValueTypes ReturnType;
    std::vector<QSharedPointer<VariableSymbol>> FunctionParameters;
    std::unique_ptr<SimpleNode> FunctionStatement;
};

#endif // FUNCTIONSYMBOL_H
