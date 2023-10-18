#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

typedef enum{
    TK_RESERVED, //symbol
    TK_NUM,      //number
    TK_EOF,      //end symbol
} TokenKind;

typedef enum{
    ND_ADD, // +
    ND_SUB, // -
    ND_MUL, // *
    ND_DIV, // /
    ND_NUM, // number
} NodeKind;

typedef struct Node Node;
typedef struct Token Token;

struct Node{
    NodeKind kind; // kind of node
    Node *l;
    Node *r;
    int val;
};

Node *new_node(NodeKind kind,Node *l,Node *r){
    Node *cur = calloc(1, sizeof(Node));
    cur->kind = kind;
    cur->l = l;
    cur->r = r;
    return cur;
}

Node *new_node_num(int val){
    Node *cur = calloc(1,sizeof(Node));
    cur->kind = ND_NUM;
    cur->val = val;
    return cur;
}

Node *expr();
Node *mul();
Node *unary();
Node *term();

Token *token;
char *user_input;

void error(char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc,char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    int pos = loc-user_input;
    fprintf(stderr, "%s\n",user_input);
    fprintf(stderr, "%*s",pos,"");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

struct Token{
    TokenKind kind;
    Token *next;
    int val;
    char *str;
};

Token *__new_token(TokenKind tokenKind, Token *cur, char *str){
    Token *token = calloc(1,sizeof(Token));
    token->kind = tokenKind;
    token->str = str;
    cur->next = token;
    return token;
}
Token *__tokenize(){
    char *p = user_input;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p){
        if(isspace(*p)){
            p++;
            continue;;
        }
        if(strchr("+-*/()", *p)){
            cur = __new_token(TK_RESERVED, cur, p++);
            continue;
        }
        if(isdigit(*p)){
            cur = __new_token(TK_NUM, cur, p);
            cur->val = strtol(p, &p, 10);
            continue;
        }
        error_at(p,"tokenize failed.");
    }
    __new_token(TK_EOF, cur, p);
    return head.next;
}

void expect(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op) error_at(token->str,"not is %c",op);
    token = token->next;
}

int expect_number(){
    if(token->kind != TK_NUM) error_at(token->str,"Not a number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof(){
    return token->kind == TK_EOF;
}

bool consume(char op){
    if(token->kind != TK_RESERVED || token->str[0] != op) return false;
    token = token->next;
    return true;
}

/*
 expr = mul ('+' mul | '-' mul) *
 mul = unary ('*' unary | '/' unary) *
 unary = ("+" unary | "-" unary)? term
 term = num | '(' expr ')'
*/

Node *expr(){
    Node *cur = mul();
    for(;;){
        if(consume('+')) cur = new_node(ND_ADD, cur, mul());
        else if(consume('-')) cur = new_node(ND_SUB, cur, mul());
        else return cur;
    }
}

Node *mul(){
    Node *node = unary();
    for(;;){
        if(consume('*')) node = new_node(ND_MUL, node, unary());
        else if(consume('/')) node = new_node(ND_DIV, node, unary());
        else return node;
    }
}
Node *unary(){
    if(consume('+')) return unary();
    if(consume('-')) return new_node(ND_SUB,new_node_num(0),unary());
    return term();
}

Node *term(){
    if(consume('(')){
        Node *cur = expr();
        expect(')');
        return cur;
    }
    return new_node_num(expect_number());
}

void gen(Node *node){
    if(node->kind == ND_NUM){
        printf("  push %d\n",node->val);
        return;
    }

    gen(node->l);
    gen(node->r);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch(node->kind){
        case ND_ADD:
            printf("  add rax, rdi\n");
            break;
        case ND_SUB:
            printf("  sub rax, rdi\n");
            break;
        case ND_MUL:
            printf("  imul rax, rdi\n");
            break;
        case ND_DIV:
            printf("  cqo\n");
            printf("  idiv rdi\n");
            break;
    }

    printf("  push rax\n");
}


int main(int argc,char **argv){
    if(argc != 2){
        error("The numbers of arguments are wrong");
        return  1;
    }

    user_input = argv[1];
    token = __tokenize();
    Node *cur_node = expr();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(cur_node);

    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}