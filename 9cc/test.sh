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
try 47 '5+6*7'
try 15 '5*(9-6)'
try 4 '(3+5)/2' 
try 13 '+55+(-6)*(+7)'
try 10 '-10+20'
try 10 '- -10'
try 10 '- - +10'
try 10 '- - - - - - - - - - - - +10'
try 0 '0==1'
try 1 '42==42'
try 1 '0!=1'
try 0 '42!=42'

try 1 '0<1'
try 0 '1<1'
try 0 '2<1'
try 1 '0<=1'
try 1 '1<=1'
try 0 '2<=1'

try 1 '1>0'
try 0 '1>1'
try 0 '1>2'
try 1 '1>=0'
try 1 '1>=1'
try 0 '1>=2'

echo OK