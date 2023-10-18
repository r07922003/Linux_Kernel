# 9cc

This project is a pratical walkthrough, following the steps outlined int the e-book ["C編譯器入門～想懂低階系統從自幹編譯器開始～"](https://koshizuow.gitbook.io/compilerbook/) to gradully develope a deeper understanding.

Translator: 高至賢（Jason Kao）

---
Title of the Original Book: [低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook)

Original Author: Rui Ueyama <ruiu@cs.stanford.edu>

Original Github: https://github.com/rui314/chibicc

---
What skills i learned:

1. Implements customized error log function
    - More deeper understanding of **printf**, **fprintf**, **vfprintf** and **va_list**.
    - ```printf("%*s",4,"hellow") <=> printf("%4s","hellow")```
      ```"_,_,_,_,hellow"```
      Using **'*'** is a nice way to put some spaces in your file, avoiding a loop.
2. Generate syntax tree by EBNF (Extended Backus–Naur form)
    -  expr = equality
    -  equality = relational ("==" relational | "!=" relational) $*$
    -  relational = add ("<" add | "<=" add | ">" add | ">=" add) $*$
    - add = mul ("+" mul | "-" mul) $*$
    - mul = unary (* unary | "/" unary) $*$
    - unary = ("+"  | "-") $?$ term
    - term = num | ( expr )
3. Using x86-64 push and pop instructions to simulate the stack due to implementation via recursion.