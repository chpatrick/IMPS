#!/bin/bash

if [ -d tmp ]; then
  echo "tmp directory exists, remove before continuing"
  return 1
fi

mkdir tmp

echo ">>>> TESTING ASSEMBLER <<<<"

exitcode=0
warnings=0

for p in demos/*.s;
do
  echo "$p:"
  name="`basename $p .s`.oout"
  assembler/assemble $p tmp/$name -q
  exitcode=$(( $exitcode + $? ))
  if [ -e demos/$name ]; then
    diff demos/$name tmp/$name
  else
    echo ">>>> $name does not exist, skipping diff <<<<"
    warnings=$(( $warnings + 1 ))
  fi
  exitcode=$(( $exitcode + $? ))
done

echo ">>>> TESTING EMULATOR <<<<"

for p in demos/*.oout;
do
  echo "$p:"
  name="`basename $p .oout`.res"
  emulator/emulate $p > tmp/$name
  exitcode=$(( $exitcode + $? ))
  if [ -e demos/$name ]; then
    diff demos/$name tmp/$name
  else
    echo ">>>> $name does not exist, skipping diff <<<<"
  fi
  exitcode=$(( $exitcode + $? ))
done

if [ $exitcode -eq 0 ]; then
  echo ">>>> ALL TESTS PASSED <<<<"
  rm -r tmp
else
  echo ">>>> ERRORS DETECTED, CHECK ./tmp/ <<<<"
fi

if [ $warnings -ne 0 ]; then
  echo ">>>> $warnings WARNINGS <<<<"
fi
