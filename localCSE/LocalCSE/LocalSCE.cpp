#include "LocalCSE.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SetVector.h"

// void replaceOp(Instruction* reinst, Instruction* inst, BasicBlock* BB);
char LocalCSE::ID = 0;

//std::set<opTable, Instructioin*> instTable;  //set of inst table with with key= (op,op1,op2) value= Instruction*
SmallSetVector<Instruction*, 16> checkList;    //dynamic instruction check list of existing operation

static RegisterPass<LocalCSE> X("myLocalCSE", "My local CSE Pass", false, false);

bool LocalCSE::runOnFunction(Function &F) {

bool change = false;  //any change in pass

//check and replace operand in BB
  for (auto b = F.begin(); b != F.end(); ++b) {
    BasicBlock &BB = *b;
    for (auto i = BB.begin(); i != BB.end(); ++i) {
      if(checkList.empty()){  // if check list is empty then insert
        checkList.insert(&*i);
      }
      else{
        bool inList = false;
        for(auto t = checkList.begin(), t != checkList.end(), ++t){
          if(i->isIdenticalTo(&*t)){  //replace all operand i in BB with t
            
            // if binary op then replace all operand
            if(i->isBinaryOp()){
              if(Value *OpI = dyn_cast<Value*>(i)){
              OpI->replaceAllUsesWith(dyn_cast<Value*>(t));
              }
              inList = true;
              change = true;
              break;
            }
            // load op check no save and replace 
            
          }
        }
        if(!inList){  //insert check list if not in list
          checkList.insert(&*i);
        }
      }

    }
  }
return change;
}

// replace operand with repeated instruction
// void replaceOp(Instruction* reinst, Instruction* inst, BasicBlock* BB){

// for (auto i = BB.begin(); i != BB.end(); ++i) {
//   for(unsigned k = 0, e = i->getNumOperands(); k!= e; ++k){
//     Value *OpI = i->getOperand(k);
//     OpI.replaceAllUsesWith();

//   }
// }

}
