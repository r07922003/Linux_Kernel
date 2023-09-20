#!/bin/bash
try(){
    expected="$1"
    input="$2"

    ./9cc "$input" > temp.s
    gcc -o temp temp.s
    ./temp
    actual="$?"

    if [ "$actual" = "$expected" ]; then
      echo "$input => $actual"
    else
      echo "$input => $expected expected,but got $actual"
      exit 1
    fi
}

try 0 0
try 42 42
try 45 '42+7-4'
try 10 '1+1+1+7'
try 11 " 12 + 34 - 35 "

echo OK