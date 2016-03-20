#!/bin/sh
make -C ../build
clang -emit-llvm -S a.c -o a.ll
opt -load ../build/MyDCE/libMyDCE.so -myMagicDCE -S a.ll -o a.opt.ll
clang a.opt.ll -o a