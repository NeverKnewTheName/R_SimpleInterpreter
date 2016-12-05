#include "flatopcodecompiler.h"

FlatOPCodeCompiler::FlatOPCodeCompiler(std::unique_ptr<SimpleNode> AST, QObject *parent) : QObject(parent)
{

}

SimpleInstruction::SimpleInstruction(const int label, const SimpleInstruction::Mnemonic mnemonic) :
    InstructionLabel(label),
    InstrucitonMnemonic(mnemonic)
{

}
