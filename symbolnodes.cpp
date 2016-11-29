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

VariableNode::VariableNode(VariableSymbolPtr relatedVariableSymbol) :
    RelatedVariableSymbol(relatedVariableSymbol),
    valueType(relatedVariableSymbol->getVariableType())
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

ValueNodeUniquePtr VariableNode::visit()
{
    RelatedVariableSymbol->getAssignedValue();
}

QString VariableNode::printValue() const
{
    return RelatedVariableSymbol->getIdentifier();
}

QString VariableNode::printNode() const
{
    QString NodeType("VariableNode");
    QString value = printValue();

    return QString("{(%1):(%2)}").arg(NodeType).arg(value);
}

QString VariableNode::getVariableName() const
{
    return RelatedVariableSymbol->getIdentifier();
}

VariableSymbolPtr VariableNode::getRelatedVariableSymbol() const
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

FunctionCallNode::FunctionCallNode(const QString &FunctionName, SymbolTablePtr CurSymblTable, QVector<SimpleNode> &FunctionArguments) :
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

ValueNodeUniquePtr FunctionCallNode::visit()
{
    FunctionNode *symblTblEntryValue = RelatedSymbol->GetFunctionNode();
    if(symblTblEntryValue == NULL)
    {
        return ValueNodeUniquePtr( new ValueNode());
    }
    else
    {
        return ValueNodeUniquePtr( new ValueNode(*symblTblEntryValue->visit()));
    }
}

AssignmentNode::AssignmentNode(VariableNodeUniquePtr VariableToAssign, SimpleNodeUniquePtr ValueToAssign) :
    VariableToAssign(VariableToAssign.take()),
    ValueToAssign(ValueToAssign.take()),
    valueType(this->VariableToAssign->getReturnType())
{
    if(this->VariableToAssign->getReturnType() == SimpleNode::ErrorType) valueType = ErrorType;
    if(this->ValueToAssign->getReturnType() == SimpleNode::ErrorType) valueType = ErrorType;
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

ValueNodeUniquePtr AssignmentNode::visit()
{
    VariableToAssign->getRelatedVariableSymbol()->assignValue(*ValueToAssign);

    return VariableToAssign->visit();
}

ProgramNode::ProgramNode(const QString &ProgramName, const SymbolTable &ParentSymbolTable)
{

}

void ProgramNode::addExpression(SimpleNodeUniquePtr &newExpression)
{
    ProgramExpressions.append(std::move(newExpression));
}

void ProgramNode::addFunctionDefinition(FunctionSymbolPtr newFunction)
{
    ProgramSymbolTable->addEntry(newFunction->getIdentifier(), newFunction);
}

void ProgramNode::addVariableDefinition(VariableSymbolPtr newVariable)
{
    ProgramSymbolTable->addEntry(newVariable->getIdentifier(), newVariable);
}

void ProgramNode::addReturnStatement(SimpleNodeUniquePtr ReturnStatement)
{
    //Maybe std::move??
    ReturnStatement = ReturnStatement;
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

ValueNodeUniquePtr ProgramNode::visit()
{
    const int NrOfProgramExpression = ProgramExpressions.size();

    for(int i = 0; i < NrOfProgramExpression; i++)
    {
        ProgramExpressions.at(i)->visit();
    }

    return ReturnStatement->visit();
}
