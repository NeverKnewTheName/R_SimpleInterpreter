#ifndef SIMPLEFLATCOMPILER_H
#define SIMPLEFLATCOMPILER_H

#include <QObject>
#include <memory>
#include "simplenode.h"

class SimpleFlatCompiler : public QObject
{
    Q_OBJECT

public:
    typedef enum _OPCode
    {
        //Value
        UINT8Val = (quint8)0u,
        INT8Val,
        UINT16Val,
        INT16Val,
        UINT32Val,
        INT32Val,
        DoubleVal,
        BoolVal,
        StringVal,
        Void,
        //Variable
        VariableVal,
        ConstVal,
        Assingment,
        //Operation
        //UnaryOperation,
        PreIncrement,
        PreDecrement,
        PostIncrement,
        PostDecrement,
        Positive,
        Negative,
        LogicalNegation,
        OnesComplement,
        //BinaryOperation,
        Addition,
        Subtraction,
        Multiplication,
        Division,
        Modulo,
        LogicalAnd,
        LogicalOr,
        LogicalXOr,
        Greater,
        Lower,
        Equal,
        EqualOrGreater,
        EqualOrLower,
        Unequal,
        BitwiseAnd,
        BitwiseOr,
        BitwiseXOr,
        LeftShift,
        RightShift,
        //TernaryOperation,
        ConditionalOp,
        //Function
        FunctionCall,
        FunctionEnd,
        //Control
        //Selection
        IfSelection,
        ElseSelecten,
        SwitchSelection,
        CaseLabel,
        //Iteration
        ForIteration,
        WhileIteration,
        DoIteration,
        //Escape
        ReturnEscape,
        BreakEscape,
        ContinueEscape,
        //Program
        ProgramStart,
        ProgramEnd,
        //Extra
        StackPush,
        StackPop,
        EOFCode
    }OPCode;

    explicit SimpleFlatCompiler(std::unique_ptr<SimpleNode> AST, QObject *parent = 0);

signals:

public slots:
};

#endif // SIMPLEFLATCOMPILER_H
