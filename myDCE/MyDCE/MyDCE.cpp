#include "MyDCE.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SetVector.h"

char MyDCE::ID = 0;
// std::vector<InstMark> instlist;    //total inst with mark for DCE detection
std::map<Instruction*, int> instlist;  //map of total inst with mark
SmallSetVector<Instruction*, 16> worklist;    //dynamic instruction worklist to check

static RegisterPass<MyDCE> X("myMagicDCE", "My DCE Pass", false, false);

bool MyDCE::runOnFunction(Function &F) {
//save inst list and mark worklist
  for (auto b = F.begin(); b != F.end(); ++b) {
    BasicBlock &BB = *b;
    for (auto i = BB.begin(); i != BB.end(); ++i) {
      int mark = 0;
      if(i->mayHaveSideEffects() || i->isTerminator()){  //check if Inst is critical
        mark = 1;  //mark critical
        worklist.insert(&*i);  //save to worklist
      }
      instlist.insert(std::pair<Instruction*, int>(&*i, mark));
    }
  }

//loop in worklist to mark  
  while(!worklist.empty()){
    Instruction *checkInst = worklist.pop_back_val();  //
    for(auto op = checkInst->op_begin(); op != checkInst->op_end(); ++op){
      if(isa<Instruction>(op->get())){  //operand in checkInst is Inst
        Instruction *useInst = static_cast<Instruction*>(op->get());
        std::map<Instruction*, int>::iterator tempInst = instlist.find(useInst);
        if(tempInst != instlist.end()){
          tempInst->second = 1;
          worklist.insert(useInst);
        }
      }
    }
  }

//delete dead code
  bool change = false;
  for(std::map<Instruction*, int>::iterator it=instlist.begin();it!=instlist.end();++it)
  {
    if(it->second == 0){
      it->first->removeFromParent();
      change = true;
    }
  }
  return change;
}