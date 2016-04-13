#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
using namespace llvm;

// class opTable{
// public:
//   unsigned opcode;
//   Use* operandList;
//   unsigned numOperand;
// };

class LocalCSE : public FunctionPass {
 public:
  static char ID;
  LocalCSE() : FunctionPass(ID) {}
  bool runOnFunction(Function &F) override;
};