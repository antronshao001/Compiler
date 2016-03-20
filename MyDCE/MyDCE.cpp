#include "MyDCE.h"
char MyDCE::ID = 0;
static RegisterPass<MyDCE> X("myMagicDCE", "My DCE Pass", false, false);
bool MyDCE::runOnFunction(Function &F) {
  for (auto b = F.begin(); b != F.end(); ++b) {
    BasicBlock &BB = *b;
    for (auto i = BB.begin(); i != BB.end(); ++i) {
      Instruction &Inst = *i;
      Inst.dump();
    }
}
  return false;
}