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


echo OK