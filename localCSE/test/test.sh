#!/bin/sh
make -C ../build
clang -emit-llvm -S a.c -o a.ll
opt -load ../build/LocalCSE/libLocalCSE.so -myLocalCSE -S a.ll -o a_cse.ll
opt -load ../../myDCE/build/MyDCE/libMyDCE.so -myMagicDCE -S a_cse.ll -o a_dce.ll
clang a_dce.ll -o aout
#opt -O3 -S a.ll -o afast.ll
#clang afast.ll -o afast
