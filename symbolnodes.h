#ifndef SYMBOLNODES_H
#define SYMBOLNODES_H

#include "simpleast.h"
#include "simplesymboltable.h"

class VariableNode : public SimpleNode
{
public:
    VariableNode(VariableSymbolPtr relatedVariableSymbol);
    ~VariableNode();
    virtual NodeType getNodeType() const;
    virtual SimpleNode::ValueTypes getReturnType() const;

    ValueNodeScopedPtr visit();

    QString printValue() const;
    virtual QString printNode() const;

    QString getVariableName() const;

    VariableSymbolPtr getRelatedVariableSymbol() const;

private:
    VariableSymbolPtr RelatedVariableSymbol;
};

typedef QScopedPointer<VariableNode> VariableNodeScopedPtr;

class DataNode : public VariableNode
{
public:
    DataNode(const unsigned int dataIndex, const SymbolTablePtr GlobalSymbolTable);
    ~DataNode();
    NodeType getNodeType() const;
    SimpleNode::ValueTypes getReturnType() const;

    QString printNode() const;

private:
    const unsigned int dataIndex;
};

//class FunctionNode : public SimpleNode
//{
//public:
//    FunctionNode(
//            QString FunctionName,
//            QVector<VariableNode *> ParametersInOrder,
//            SimpleNode::ValueTypes returnType,
//            SymbolTable *SubSymbolTable
//            );
//    ~FunctionNode();

//    void addFuncExpressions(QVector<SimpleNode *> FuncExpressions);
//    void addReturnStatement(SimpleNode *returnNode);
//    void addAssignment(AssignmentNode *paramAssignment);

//    SymbolTable *getFuncSymbolTable() const;

//    QVector<VariableNode *> getParametersInOrder() const;

//    QString getFunctionName() const;

//    // SimpleNode interface
//public:
//    NodeType getNodeType() const;
//    ValueTypes getReturnType() const;
//    QString printValue() const;
//    QString printNode() const;
//    const ValueNode &visit();

//private:
//    ValueNode Result;
//    QString FunctionName;
//    const QVector<VariableNode *> ParametersInOrder;
//    QVector<AssignmentNode*> ParameterAssignments;
//    SimpleNode::ValueTypes returnType;
//    QVector<SimpleNode *> FuncExpressions;
//    SimpleNode *returnNode;
//    SymbolTable * const FuncSymbolTable;
//};

class FunctionCallNode : public SimpleNode
{
public:
    FunctionCallNode(const QString &FunctionName, SymbolTableEntryPtr CurSymblTable, QVector<SimpleNodeScopedPtr> FunctionArguments = QVector<SimpleNodeScopedPtr>());
    ~FunctionCallNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeScopedPtr visit();

private:
    QString FunctionName;
    QVector<SimpleNode*> FuncParams;
    FunctionSymbol *RelatedSymbol;
    SimpleNode::ValueTypes returnType;
};

class AssignmentNode : public SimpleNode
{
public:
//    AssignmentNode();
    AssignmentNode(VariableNodeScopedPtr VariableToAssign, SimpleNodeScopedPtr ValueToAssign);
    ~AssignmentNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeScopedPtr visit();

private:
    VariableNodeScopedPtr VariableToAssign;
    SimpleNodeScopedPtr ValueToAssign;
};


class ProgramNode : public SimpleNode
{
public:
    ProgramNode(
            const QString &ProgramName,
            const SymbolTable &ParentSymbolTable
            );
    ~ProgramNode();

    void addExpression(const SimpleNode &newExpression);
    void addFunctionDefinition(const FunctionSymbol &newFunction);
    void addVariableDefinition(const VariableSymbol &newVariable);
    void addReturnStatement(const SimpleNode &ReturnStatement);

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeScopedPtr visit();

private:
    SimpleNode::ValueTypes type;
    QVector<SimpleNodeScopedPtr> ProgramExpressions;
    SimpleNodeScopedPtr ReturnStatement;
    const SymbolTablePtr ParentSymbolTable;
    SymbolTablePtr ProgramSymbolTable;
};

#endif // SYMBOLNODES_H
