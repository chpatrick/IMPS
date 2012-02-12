#!/bin/bash

PATH=/vol/project/2010/1203/g10120303:$PATH
rm -rvf final
mkdir -p final/{fancy,}/{assembler,emulator}
for file in {assembler,emulator}/*.{c,h}
do
  echo $file
  coan source -UFANCY $file > final/$file
  coan source -DFANCY $file > final/fancy/$file
done
