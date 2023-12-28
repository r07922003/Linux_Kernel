#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

//
// tokenize.c
//


// Token
typedef enum{
    TK_RESERVED, //symbol
    TK_NUM,      //number
    TK_EOF,      //end symbol
} TokenKind;

// Toke type
typedef struct Token Token;
struct Token{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
    int len;
};

void error(char *fmt, ...);
void error_at(char *loc,char *fmt, ...);
void expect(Token** token,char *op);
int expect_number(Token **token);
bool consume(Token **token,char *op);
bool startswith(char *a, char *b);
Token *__new_token(TokenKind tokenKind, Token *cur, char *str,int len);
Token *__tokenize(char *p);

//
// parse.c
//

// AST node
typedef enum{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_NUM, // Integer
} NodeKind;

//AST node type
typedef struct Node Node;
struct Node{
    NodeKind kind; // kind of node
    Node *l;
    Node *r;
    int val;
};

Node *new_node(NodeKind kind,Node *l,Node *r);
Node *expr(Token**);
Node *equality(Token**);
Node *relational(Token**);
Node *add(Token**);
Node *mul(Token**);
Node *unary(Token**);
Node *term(Token**);
void gen(Node *node);

//
// main.c
//

extern char *user_input;