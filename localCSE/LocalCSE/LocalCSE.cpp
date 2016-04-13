#include "LocalCSE.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/Instructions.h"

// void replaceOp(Instruction* reinst, Instruction* inst, BasicBlock* BB);
char LocalCSE::ID = 0;
void plist(SmallVector<Instruction*, 16> printList);
//std::set<opTable, Instructioin*> instTable;  //set of inst table with with key= (op,op1,op2) value= Instruction*
SmallVector<Instruction*, 16> checkList;    //dynamic instruction check list of existing operation
//std::list<Instruction*> checkList;

static RegisterPass<LocalCSE> X("myLocalCSE", "My local CSE Pass", false, false);

bool LocalCSE::runOnFunction(Function &F) {

bool change = false;  //any change in pass

//check and replace operand in BB
  for (auto b = F.begin(); b != F.end(); ++b) {
    BasicBlock &BB = *b;
    for (auto i = BB.begin(); i != BB.end(); ++i) {
      if(checkList.empty()){  // if check list is empty then insert
        checkList.push_back(&*i);
      }
      else{
        bool inList = false;
        for(auto t = checkList.begin(); t != checkList.end(); ++t){ 
          Instruction* reInst = *t; 
          if(i->isIdenticalTo(reInst)){  //replace all operand i in BB with t
            // if binary op then replace all operand
            if(i->isBinaryOp()){
                Value *OpI = dyn_cast<Value>(&*i);
                Value *OpR = dyn_cast<Value>(&*reInst);
                OpI->replaceAllUsesWith(OpR);
                inList = true;
                change = true;
                break;
            }
            else{
              // if load inst check any sotre between load 
              if(llvm::LoadInst* inst = dyn_cast<LoadInst>(i)){
                bool havestore = false;  // not being stored then its ok to replace load
                for(auto checkI=checkList.begin();checkI != checkList.end();++checkI)
                  if((*checkI)->getOpcode() == 31) // store instruction
                    if((*checkI)->getOperand(1) == inst->getOperand(0))
                      havestore = true;
                //replace load inst
                if(!havestore){
                  Value *OpI = dyn_cast<Value>(&*i);
                  Value *OpR = dyn_cast<Value>(&*reInst);
                  OpI->replaceAllUsesWith(OpR);
                  inList = true;
                  change = true;
                }
              }
            }
          }
        }
        if(!inList){  //insert check list if not in list
          checkList.push_back(&*i);
        }
      }
    }
  }
plist(checkList);
return change;
}

void plist(SmallVector<Instruction*, 16> printList){
  for(auto printInst=printList.begin();printInst != printList.end();printInst++){
    (*printInst)->dump();
  }
}

// replace operand with repeated instruction
// void replaceOp(Instruction* reinst, Instruction* inst, BasicBlock* BB){

// for (auto i = BB.begin(); i != BB.end(); ++i) {
//   for(unsigned k = 0, e = i->getNumOperands(); k!= e; ++k){
//     Value *OpI = i->getOperand(k);
//     OpI.replaceAllUsesWith();

//   }
// }
