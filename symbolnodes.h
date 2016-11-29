#ifndef SYMBOLNODES_H
#define SYMBOLNODES_H

#include "simpleast.h"
#include "simplesymboltable.h"

class VariableNode : public SimpleNode
{
public:
    VariableNode(VariableSymbolPtr relatedVariableSymbol);
    virtual ~VariableNode();
    virtual NodeType getNodeType() const;
    virtual SimpleNode::ValueTypes getReturnType() const;

    ValueNodeUniquePtr visit();

    QString printValue() const;
    virtual QString printNode() const;

    QString getVariableName() const;

    VariableSymbolPtr getRelatedVariableSymbol() const;

private:
    VariableSymbolPtr RelatedVariableSymbol;
};

typedef std::unique_ptr<VariableNode> VariableNodeUniquePtr;

class DataNode : public VariableNode
{
public:
    DataNode(const unsigned int dataIndex, const SymbolTablePtr GlobalSymbolTable);
    virtual ~DataNode();
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
//    virtual ~FunctionNode();

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
    FunctionCallNode(const QString &FunctionName, SymbolTableEntryPtr CurSymblTable, QVector<SimpleNodeUniquePtr> &FunctionArguments);
    virtual ~FunctionCallNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeUniquePtr visit();

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
    AssignmentNode(VariableNodeUniquePtr VariableToAssign, SimpleNodeUniquePtr ValueToAssign);
    virtual ~AssignmentNode();

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeUniquePtr visit();

private:
    VariableNodeUniquePtr VariableToAssign;
    SimpleNodeUniquePtr ValueToAssign;
};


class ProgramNode : public SimpleNode
{
public:
    ProgramNode(
            const QString &ProgramName,
            const SymbolTable &ParentSymbolTable
            );
    virtual ~ProgramNode();

    void addExpression(SimpleNodeUniquePtr &newExpression);
    void addFunctionDefinition(FunctionSymbolPtr newFunction);
    void addVariableDefinition(VariableSymbolPtr newVariable);
    void addReturnStatement(SimpleNodeUniquePtr ReturnStatement);

    // SimpleNode interface
public:
    NodeType getNodeType() const;
    ValueTypes getReturnType() const;
    QString printValue() const;
    QString printNode() const;
    ValueNodeUniquePtr visit();

private:
    SimpleNode::ValueTypes type;
    QVector<SimpleNodeUniquePtr> ProgramExpressions;
    SimpleNodeUniquePtr ReturnStatement;
    const SymbolTablePtr ParentSymbolTable;
    SymbolTablePtr ProgramSymbolTable;
};

typedef std::unique_ptr<ProgramNode> ProgramNodeUniquePtr;

#endif // SYMBOLNODES_H
