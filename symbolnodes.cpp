#include "symbolnodes.h"

DataNode::DataNode(const unsigned int dataIndex, const SymbolTablePtr GlobalSymbolTable) :
    VariableNode(GlobalSymbolTable->getParentSymbolTable() != NULL ? /*NULL*/ : QString("D%1").arg(dataIndex), GlobalSymbolTable),
    dataIndex(dataIndex),
    valueType(ValueNode::Integer)
{
    if( GlobalSymbolTable->getParentSymbolTable() != NULL )
    {
        qDebug() << "TRIED TO SET A DATANODE WITH A SYMBOL TABLE THAT WAS NOT THE GLOBAL SYMBOL TABLE!";
    }
}

DataNode::~DataNode()
{
    qDebug() << __PRETTY_FUNCTION__;
    qDebug() << printNode();
}

SimpleNode::NodeType DataNode::getNodeType() const
{
    return SimpleNode::Data;
}

SimpleNode::ValueTypes DataNode::getReturnType() const
{
    return valueType;
}

QString DataNode::printNode() const
{
    QString NodeType = "DataNode";
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

VariableNode::VariableNode() :
    VariableName(VariableName),
    ScopedSymbolTable(ScopedSymbolTable)
{

}

VariableNode::VariableNode(const QString &VariableName, SymbolTablePtr ScopedSymbolTable) :
    VariableName(VariableName),
    ScopedSymbolTable(ScopedSymbolTable),
    valueType(qSharedPointerDynamicCast<VariableSymbol>(ScopedSymbolTable.lookup(VariableName))->getReturnType())
{
}

VariableNode::~VariableNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType VariableNode::getNodeType() const
{
    return SimpleNode::Variable;
}

SimpleNode::ValueTypes VariableNode::getReturnType() const
{
    valueType = qSharedPointerDynamicCast<VariableSymbol>(ScopedSymbolTable.lookup(VariableName))->getReturnType();
    return valueType;
}

ValueNodeScopedPtr VariableNode::visit()
{
    return ValueNodeScopedPtr(new ValueNode(*(qSharedPointerDynamicCast<VariableSymbol>(ScopedSymbolTable.lookup(VariableName))->getAssignedValue())));
}

QString VariableNode::printValue() const
{
    return QString(VariableName);
}

QString VariableNode::printNode() const
{
    QString NodeType("VariableNode");
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

QString VariableNode::getVariableName() const
{
    return VariableName;
}

VariableSymbol *VariableNode::getRelatedVariableSymbol() const
{
    return RelatedVariableSymbol;
}


//FunctionNode::FunctionNode(QString FunctionName, QVector<VariableNode *> ParametersInOrder, SimpleNode::ValueTypes returnType, SymbolTable *SubSymbolTable) :
//    FunctionName(FunctionName),
//    ParametersInOrder(ParametersInOrder),
//    returnType(returnType),
//    FuncSymbolTable(SubSymbolTable)
//{
//}

//FunctionNode::~FunctionNode()
//{
//    qDebug() << __PRETTY_FUNCTION__;
//}

//void FunctionNode::addFuncExpressions(QVector<SimpleNode *> FuncExpressions)
//{
//    this->FuncExpressions = FuncExpressions;
//}

//void FunctionNode::addReturnStatement(SimpleNode *returnNode)
//{
//    if( ( (returnNode == NULL) && ( returnType != SimpleNode::Void ) ) || ( !SimpleNode::canConvertTypes(returnNode->getReturnType(), returnType) ) )
//    {
//        this->returnType = SimpleNode::ErrorType;
//        this->returnNode = NULL;
//    }

//    this->returnNode = returnNode;
//}

//void FunctionNode::addAssignment(AssignmentNode *paramAssignment)
//{
//    ParameterAssignments.append(paramAssignment);
//}

//SimpleNode::NodeType FunctionNode::getNodeType() const
//{
//    return SimpleNode::Function;
//}

//SimpleNode::ValueTypes FunctionNode::getReturnType() const
//{
//    return returnType;
//}

//QString FunctionNode::printValue() const
//{
//    QString funcParams = QString("(");

//    for(VariableNode *param : this->ParametersInOrder)
//    {
//        funcParams.append(QString(" %1 %2").arg(SimpleNode::getHumanReadableTypeNameToValueType(param->getReturnType())).arg(param->getVariableName()));
//    }

//    funcParams.append(" )");
//    return QString("%1 %2 %3").arg(SimpleNode::getHumanReadableTypeNameToValueType(returnType)).arg(FunctionName).arg(funcParams);
//}

//QString FunctionNode::printNode() const
//{
//    return QString("{(FunctionNode):(%1)}").arg(printValue());
//}

//const ValueNode &FunctionNode::visit()
//{
//    for(AssignmentNode *assignment : ParameterAssignments)
//    {
//        assignment->visit();
//    }

//    for(SimpleNode *expr : FuncExpressions)
//    {
//        ValueNode exprRes = expr->visit();
//        qDebug() << "FunctionNode visited --> " << exprRes.printNode();
//    }

//    switch(returnType)
//    {
//    case SimpleNode::Integer:
//        Result = ValueNode(returnNode->visit().getValue().value<int>());
//        break;
//    case SimpleNode::Double:
//        Result = ValueNode(returnNode->visit().getValue().value<double>());
//        break;
//    case SimpleNode::Bool:
//        Result = ValueNode(returnNode->visit().getValue().value<bool>());
//        break;
//    case SimpleNode::String:
//        Result = ValueNode(returnNode->visit().getValue().value<QString>());
//        break;
//    case SimpleNode::Void:
//    default:
//        Result = ValueNode();
//    }

//    return Result;
//}

//QString FunctionNode::getFunctionName() const
//{
//    return FunctionName;
//}

//QVector<VariableNode *> FunctionNode::getParametersInOrder() const
//{
//    return ParametersInOrder;
//}

//SymbolTable *FunctionNode::getFuncSymbolTable() const
//{
//    return FuncSymbolTable;
//}

FunctionCallNode::FunctionCallNode(const QString &FunctionName, SymbolTablePtr CurSymblTable, QVector<SimpleNode> FunctionArguments) :
    FunctionName(FunctionName),
    FuncParams(FunctionParameters)
{
    FunctionSymbolPtr functionPtr = qSharedPointerDynamicCast<FunctionSymbol>(CurSymblTable->lookup(FunctionName));
    if(RelatedSymbol == NULL)
    {
        returnType = SimpleNode::ErrorType;
    }
    else
    {
        FunctionNode *funcNode = RelatedSymbol->GetFunctionNode();
        if(funcNode == NULL)
        {
            returnType = SimpleNode::ErrorType;
            return;
        }
        QVector<VariableNode *> paramsInOrder = funcNode->getParametersInOrder();
        int NrOfPassedFuncParams = FunctionParameters.size();
        if(NrOfPassedFuncParams != FunctionParameters.size())
        {
            returnType = SimpleNode::ErrorType;
            return;
        }
        for(int i = 0; i < NrOfPassedFuncParams; i++)
        {
            VariableNode *paramDecl = paramsInOrder.at(i);
            SimpleNode *paramDef = FunctionParameters.at(i);
            if(!SimpleNode::canConvertTypes(paramDef->getReturnType(),paramDecl->getReturnType()))
            {
                returnType = SimpleNode::ErrorType;
                return;
            }
            funcNode->addAssignment(new AssignmentNode(paramDecl,paramDef));
            //            paramDecl->setAssignment(paramDef);
        }
        returnType = funcNode->getReturnType();
    }
}

FunctionCallNode::~FunctionCallNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType FunctionCallNode::getNodeType() const
{
    return SimpleNode::FunctionCall;
}

SimpleNode::ValueTypes FunctionCallNode::getReturnType() const
{
    return returnType;
}

QString FunctionCallNode::printValue() const
{
    return QString("%1()").arg(FunctionName);
}

QString FunctionCallNode::printNode() const
{
    return QString("{FunctionCall}:{%1}").arg(printValue());
}

ValueNodeScopedPtr FunctionCallNode::visit()
{
    FunctionNode *symblTblEntryValue = RelatedSymbol->GetFunctionNode();
    if(symblTblEntryValue == NULL)
    {
        return ValueNodeScopedPtr( new ValueNode());
    }
    else
    {
        return ValueNodeScopedPtr( new ValueNode(*symblTblEntryValue->visit()));
    }
}

AssignmentNode::AssignmentNode()
{

}

AssignmentNode::AssignmentNode(const VariableNode &VariableToAssign, const SimpleNode &ValueToAssign) :
    VariableToAssign(VariableToAssign),
    //    ValueToAssign(SimpleNodePtr(new SimpleNode(ValueToAssign))
{
}

AssignmentNode::~AssignmentNode()
{
    qDebug() << __PRETTY_FUNCTION__;
}

SimpleNode::NodeType AssignmentNode::getNodeType() const
{
    return SimpleNode::Assignment;
}

SimpleNode::ValueTypes AssignmentNode::getReturnType() const
{
    return VariableToAssign->getReturnType();
}

QString AssignmentNode::printValue() const
{
    return QString("%1 = %2").arg(VariableToAssign->printValue()).arg(ValueToAssign->printValue());
}

QString AssignmentNode::printNode() const
{
    return QString("{AssignmentNode}:{%1}").arg(printValue());
}

ValueNodeScopedPtr AssignmentNode::visit()
{
    VariableToAssign->setAssignment(ValueToAssign);

    return ValueNodeScopedPtr( new ValueNode(*VariableToAssign->visit()));
}



ProgramNode::ProgramNode(const QString &ProgramName, const SymbolTable &ParentSymbolTable)
{

}

SimpleNode::NodeType ProgramNode::getNodeType() const
{
    return SimpleNode::Program;
}

SimpleNode::ValueTypes ProgramNode::getReturnType() const
{
    return type;
}

QString ProgramNode::printValue() const
{
    return QString("%1").arg(Result.visit().getValue().value<QString>());
}

QString ProgramNode::printNode() const
{
    return QString("{ProgramNode}:{%1}").arg(printValue());
}

ValueNodeScopedPtr ProgramNode::visit()
{
    for(AssignmentNode &&assign : ProgramAssignments)
    {
        //        assign.
    }

    return ValueNodeScopedPtr( new ValueNode() );
}
