# Linux Kernel Learning
此筆記為單純紀錄作者在 Linux Kernel 以及 C語言的學習歷程/歷史，並非技術性探討，若有錯誤還請見諒並貴求指正。

---
Future Work:
1.[現代硬體架構上的演算法: 以 Binary Search 為例](https://hackmd.io/@RinHizakura/BJ-Zjjw43)
2.[High-Performance Metaprogramming - Template Metaprogramming in C++](https://www.youtube.com/watch?v=0mFGybZaQ54)


[TOC]
# Tool
- Trace code tool: [**bootlin**](https://elixir.bootlin.com/linux/latest/source)
- Trace instrunction set: [**Ripes**](https://github.com/mortbopet/Ripes)
## Setting ubuntu environment
- [x] Ubuntu Linux 22.04-LTS 
- [x] purf related stuff
- [ ] gnuplot realted stuff

## Onine Course:
- [Intro to Computer Systems (from jserv CS:APP 學習指引緒論)](https://hackmd.io/@sysprog/CSAPP/https%3A%2F%2Fhackmd.io%2Fs%2FSJ7V-qikG?type=book)
    - [x] Chapter 1 Overview 
    - [x] Chapter 2.1 Bits, Bytes, & Integers I
    - [ ] Chapter 2.2-2.3 Bits, Bytes, & Integers II
    - [ ] Cahpter 3.1-3.5 Machine Prog: Basics
- [解讀計算機編碼](https://hackmd.io/@sysprog/binary-representation)
    - [x] 1's compoment & 2's compoment
    - [x] 圖解有限長度二進位運算與二補數編碼
    - [x] 不需要分支的設計 by bit operation(持續努力implement+更多用法) 
        - abs function
            - x >= 0 => mask = x>>31 = 0
            - x <0 => mask = x>>31 = -1 
            - If mask = -1 (0x11111...1)
                - x ^ mask = ~x (以時鐘圖來觀看的話，在一補數空間轉換)
                - x - 1 (以時鐘圖來觀看的話，為往左移一格)
        - bigger & equal
        - smaller & equal
- 你所不知道的 C 語言 by jserv:
    - [ ] [開發工具和規格標準](https://hackmd.io/@sysprog/c-standards?type=view)
        - [ ] [ISO/IEC 9899 (a.k.a C99 Standard)](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)
        - [ ] GDB for debugging
    - [ ] 你所不知道的 C 語言: bitwise 操作
        - [x] Bitwise operator
        - [x] Set / Clear / Toggle / RightMost / LeftMost bit
        - [ ] popcount
            - [Fast Bit Counting](https://gurmeet.net/puzzles/fast-bit-counting-routines/)
            - [The bit twiddler](https://bits.stephan-brumme.com/countBits.html)
            - [popcount 算法分析](https://zhuanlan.zhihu.com/p/341488123)
            - [2019q3 Homework2 (quiz2)](https://hackmd.io/@yxguo/BJZ-JUJdS)
        - [ ] 影像處理和 bitwise operation (非常值得研究)
            - 在處理 Alpha blend components 時會執行以下程式碼
              ```c
              // Alpha blend components
              unsigned out_r = (fg_r * alpha + bg_r * (255 - alpha)) / 255;
              unsigned out_g = (fg_g * alpha + bg_g * (255 - alpha)) / 255;
              unsigned out_b = (fg_b * alpha + bg_b * (255 - alpha)) / 255;
              // Can tansform to below code
              unsigned out_r = fg_r * alpha + bg_r * (255 - alpha);
              unsigned out_g = fg_g * alpha + bg_g * (255 - alpha);
              unsigned out_b = fg_b * alpha + bg_b * (255 - alpha);
              out_r = (out_r + 1 + (out_r >> 8)) >> 8;
              out_g = (out_g + 1 + (out_g >> 8)) >> 8;
              out_b = (out_b + 1 + (out_b >> 8)) >> 8;
              ```
              [Why? Jim Blinn dedicated his November 1995 column in IEEE Computer Graphics and Applications to tricks for computing x/255 for 16-bit x](https://research.swtch.com/divmult)
              Because of
              - $1/255= 257/255*257=(257/256*256-1) \approx 257/(256*256)$
              - $257/(256*256)=(256+1)/(256*256)=1/256+(1/256*256)$
              - The ``+1`` is rounding support which allows the formula to exactly match the integer division x/255 for all values of x in [0..65534].
        - [ ] [2018q1 第 1 週測驗題](https://hackmd.io/@sysprog/rJh9U4Guf?type=view)
            - 如何判斷是否被整數 N 整除(以 Nmod2^m^ 模運算的循環餘數相加遞迴)
                |N mod 2^m^|1|2|4|8|16|32|64|128|256|512|1024|
                |:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:-|:-|:-|:-|
                |3|1|2|1|2|1|2|1|2|1|2|1|
                |5|1|2|4|3|1|2|4|3|1|2|4|
                |7|1|2|4|1|2|4|1|2|1|2|4|
                |11|1|2|4|8|5|10|9|7|3|6|1|
                |15|1|2|4|8|1|2|4|8|1|2|4|
            - 在 C 程式中，表達式 1 << 2 + 3 << 4 求值後為何？
              - 可透過 [C 語言規格書](https://www.open-std.org/jtc1/sc22/wg14/www/docs/n1256.pdf)中的 6-5: precdence of operators 中有以下註解：
              ``The syntax specifies the precedence of operators in the evaluation of an expression, which is the same as the order of the major subclauses of this subclause, highest precedence first.``
              - 意思也就是說運算子的優先權和它在規格書中介紹的順序先後相同，先介紹到的運算子有較高優先權。
              - 題目中的 + 在 6.5.6 Additive operators 中被定義， << 則到了 6.5.7 Bitwise shift operators 才被定義。因此必須先執行 + 再執行 <<。
          - CS:APP 中有歸納出 $x∗K$ 中的 $K$ 如果可以表示成$[1∗(111)0∗]$的 Regular Expression ，有最小唯一解。其中最高位的 1 在第 n 位，最低位的 1 在第 m 位。


    - [ ] 你所不知道的 C 語言：數值系統篇 (搭配 bitwise操作)
    - [ ] 你所不知道的 C 語言：指標篇
        - [x] 你所不知道的 C 語言：指標篇 (上) video 
        - [x] 你所不知道的 C 語言：指標篇 (下) video
        - [x] pointer to pointer
        - [x] pointer to const/ const pointer
    - [ ] 你所不知道的 C 語言：函式呼叫篇
        - [x] malloc (配置在 Heap 而非 Stack) 存取大容量的危險性，記憶體不夠用是因為對應的 Heap 空間不夠所導致的。
    - [ ] 你所不知道的 C 語言：前置處理器應用篇
        - 關於 Compiler / Interpreter / VM 的相關解釋可查看此篇文章: [一點都不深入的了解 Compiler、 Interpreter 和 VM](https://www.spreered.com/compiler_for_dummies/)
        - 動手實做[ C 編譯器入門～想懂低階系統從自幹編譯器開始～](https://koshizuow.gitbook.io/compilerbook/) 
          **譯者**: 高至賢（Jason Kao) 
          **原始作者**: Rui Ueyama ruiu@cs.stanford.edu
        - Make file 相關 resource
            - [靜態、共享與動態鏈結函式庫](http://maxubuntu.blogspot.com/2010/02/makefile.html)
            - [Make File 大全](https://jasonblog.github.io/note/index.html)
        
## Hardware related
- [Branch Patterns, Using GCC](https://cellperformance.beyond3d.com/articles/2006/04/branch-patterns-using-gcc.html)
    - **RULE #1**: FORWARD BRANCHES ARE NOT LIKELY TO BE TAKEN
    - **RULE #2**: BACKWARD BRANCHES ARE LIKELY TO BE TAKEN
        - Try to change **for** and **while loops** to **do-while loops**. This generally makes the test for the loop condition faster.
        - When converting for loops that have **continue** statements is their bodies, change those statements to an explicit **goto** to the update part of the code near **the end of the do-while loop**.
        - Change **for and while loops** where it's absolutely necessary to test for the condition at the beginning to an **if** to test the condition and a **do-while** to iterate the loop. This allows for more explicit control of branch prediction and blocking.
            - e.g.
                ```c=
                #while
                void print_list(const list_t *list){
                    while(list != NULL){
                        printf("%d\n", list->value);
                        list = list.next;
                    }
                }
                #if & do-while
                void print_list(const list_t *list){
                    if(list != NULL){
                        do{
                            printf("%d\n",list->value);
                            list = list->next;
                        }while(list!=NULL);
                    }
                }
                ```
    - **RULE #3**: SWITCH STATEMENTS (Not familiar)
        - Avoid switches for high performance code. Changes in case values can result in non-obvious performance impacts.
        - Eliminate small switches. Convert to a series of if statements contained by an outer branch (when the default case is likely). Alternatively convert to a branch-free expression.
        - Convert medium-size switches to jump tables (using GCC's &&label_name syntax).
        - Convert large, sparse switches to smaller ones by pre-processing the case values (i.e. via a hash function), then manually convert to a jump table depending on the number of cases and their likelihood.
    - **TIP #1**: DON´T CHANGE GCC´S DEFAULT STATIC PREDICTION
        - Don't use the __builtin_expect extension to change GCC's default branch prediction, rearrange your code instead. Only use this extension to convert what would have been dynamically predicted branches to statically predicted ones, and only where the data has been analyzed and the predicability is well-known in advance.
    - **TIP #2**: BOOLEAN OPERATORS AFFECT STATIC BRANCH PREDICTION
        - Avoid complex boolean expressions.
        - When cases are not equally likely, use "and" and order by most->least likely.
        - When cases are equally likely, use "or". The order is not important.
    - **TIP #3**: SIMPLE TRACES (NOT SHORTER TRACES) WILL ALMOST ALWAYS GENERATE BETTER CODE IN GCC
        - Use the LOAD->COMPARE->COMBINE pattern.
    - **TIP #4**: BITWISE OPERATORS GENERATE FEWER BRANCHES AND MICROCODED INSTRUCTIONS THAN LOGICAL OPERATORS.
        - Combine boolean results with bitwise operators.
            - e.g.
                ```c=
                #one branch can change to zero branch by bitwise operator change
                int box_test_overlap_combine_1( const Box* restrict const a, const Box* restrict const b ){
                  float acx       = a->center.e[0];
                  float acy       = a->center.e[1];
                  float acz       = a->center.e[2];
                  float aex       = a->extent.e[0];
                  float aey       = a->extent.e[1];
                  float aez       = a->extent.e[2];
                  float bcx       = b->center.e[0];
                  float bcy       = b->center.e[1];
                  float bcz       = b->center.e[2];
                  float bex       = b->extent.e[0];
                  float bey       = b->extent.e[1];
                  float bez       = b->extent.e[2];
                  float adiff_cx  = fabs( acx-bcx );
                  float adiff_cy  = fabs( acy-bcy );
                  float adiff_cz  = fabs( acz-bcz );
                  float sum_ex    = aex + bex;
                  float sum_ey    = aey + bey;
                  float sum_ez    = aez + bez;
                  int   overlap_x = adiff_cx <= sum_ex;
                  int   overlap_y = adiff_cy <= sum_ey;
                  int   overlap_z = adiff_cz <= sum_ez;
                  //int   overlap   = overlap_x && overlap_y && overlap_z; 
                  int   overlap = overlap_x & overlap_y & overlap_z;
                  return (overlap);
                }
                ```
# Projects & Imeplements
## [9cc compiler](https://github.com/r07922003/Linux_Kernel/tree/main/9cc)
1. Implements customized error log function
    - More deeper understanding of **printf**, **fprintf**, **vfprintf** and **va_list**.
    - ```printf("%*s",4,"hellow") <=> printf("%4s","hellow")```
      ```"_,_,_,_,hellow"```
      Using **'*'** is a nice way to put some spaces in your file, avoiding a loop.
2. Generate syntax tree by EBNF (Extended Backus–Naur form)
    -  expr = equality
    -  equality = relational ($"=="$ relational $|$ $"!="$ relational) $*$
    -  relational = add ($"<"$ add $|$ $"<="$ add $|$ $">"$ add $|$ $">="$ add) $*$
    - add = mul ($"+"$ mul $|$ $"-"$ mul) $*$
    - mul = unary ($"*"$ unary $|$ $"/"$ unary) $*$
    - unary = ($"+"$  $|$ $"-"$) $?$ term
    - term = num | $($ expr $)$
3. Using x86-64 push and pop instructions to simulate the stack due to implementation via recursion.