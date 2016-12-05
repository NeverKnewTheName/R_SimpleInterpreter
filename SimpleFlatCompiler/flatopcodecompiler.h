#ifndef FLATOPCODECOMPILER_H
#define FLATOPCODECOMPILER_H

#include <QObject>
#include <memory>
#include "simplenode.h"

class FlatOPCodeCompiler : public QObject
{
    Q_OBJECT
public:
    explicit FlatOPCodeCompiler(std::unique_ptr<SimpleNode> AST, QObject *parent = 0);

signals:

public slots:
};

class SimpleInstruction
{
public:

    typedef enum _Mnemonic
    {

    }Mnemonic;

    SimpleInstruction(const int label, const Mnemonic mnemonic /*operands*/);
    ~SimpleInstruction();

private:
    const int InstructionLabel;
    const Mnemonic InstrucitonMnemonic;
};

class AddInstruction : public SimpleInstruction
{
public:
    AddInstruction( const int label, const int opOne, const int opTwo );
    ~AddInstruction();
};

#endif // FLATOPCODECOMPILER_H
