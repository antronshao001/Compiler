#!/bin/sh
make -C ../build
clang -emit-llvm -S a.c -o a.ll
opt -load ../build/MyDCE/libMyDCE.so -myMagicDCE -S a.ll -o atest.ll
clang atest.ll -o atest
#opt -O3 -S a.ll -o afast.ll
#clang afast.ll -o afast
