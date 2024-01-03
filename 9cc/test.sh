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

try 0 'return 0;'
try 42 'return 42;'
try 45 'return 42+7-4;'
try 10 'return 1+1+1+7;'
try 11 "return  12 + 34 - 35 ;"
try 47 'return 5+6*7;'
try 15 'return 5*(9-6);'
try 4 'return (3+5)/2;' 
try 13 'return +55+(-6)*(+7);'
try 10 'return -10+20;'
try 10 'return - -10;'
try 10 'return - - +10;'
try 10 'return - - - - - - - - - - - - +10;'
try 0 'return 0==1;'
try 1 'return 42==42;'
try 1 'return 0!=1;'
try 0 'return 42!=42;'

try 1 'return 0<1;'
try 0 'return 1<1;'
try 0 'return 2<1;'
try 1 'return 0<=1;'
try 1 'return 1<=1;'
try 0 'return 2<=1;'

try 1 'return 1>0;'
try 0 'return 1>1;'
try 0 'return 1>2;'
try 1 'return 1>=0;'
try 1 'return 1>=1;'
try 0 'return 1>=2;'

try 1 'return 1; 2; 3;'
try 2 '1; return 2; 3;'
try 3 '1; 2; return 3;'

try 3 'a=3; return a;'
try 8 'a=3; z=5; return a+z;'
try 14 'a=3; b=5*6-8; return a+b/2;'

try 3 'foo=3; return foo;'
try 8 'foo123=3; bar=5; return foo123+bar;'
try 14 'foo=3; b=5*6-8; return foo+b/2;'

echo OK