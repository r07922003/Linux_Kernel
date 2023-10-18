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
    ND_EQ,  // ==
    ND_NE,  // !=
    ND_LT,  // <
    ND_LE,  // <=
    ND_NUM, // Integer
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
Node *equality();
Node *relational();
Node *add();
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
    int len;
};

Token *__new_token(TokenKind tokenKind, Token *cur, char *str,int len){
    Token *token = calloc(1,sizeof(Token));
    token->kind = tokenKind;
    token->str = str;
    token->len = len;
    cur->next = token;
    return token;
}

bool startswith(char *a, char *b){
    return memcmp(a,b,strlen(b)) == 0;
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
        if(startswith(p,"==") || startswith(p,"!=") || startswith(p,"<=") || startswith(p,">=")){
            cur = __new_token(TK_RESERVED, cur, p,2);
            p+=2;
            continue;
        }
        if(strchr("+-*/()<>", *p)){
            cur = __new_token(TK_RESERVED, cur, p++,1);
            continue;
        }
        if(isdigit(*p)){
            cur = __new_token(TK_NUM, cur, p,0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p-q;
            continue;
        }
        error_at(p,"tokenize failed.");
    }
    __new_token(TK_EOF, cur, p, 0);
    return head.next;
}

void expect(char *op){
    if(token->kind != TK_RESERVED || strlen(op)!= token->len || memcmp(token->str, op, token->len)) error_at(token->str,"not is %c",op);
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

bool consume(char *op){
    if(token->kind != TK_RESERVED || strlen(op)!= token->len || memcmp(token->str, op, token->len)) return false;
    token = token->next;
    return true;
}

/*
 expr = equality
 equality = relational ("==" relational | "!=" relational)*
 relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 add = mul ('+' mul | '-' mul) *
 mul = unary ('*' unary | '/' unary) *
 unary = ("+" unary | "-" unary)? term
 term = num | '(' expr ')'
*/

Node *expr(){
    return equality();
}

Node *equality(){
    Node *cur = relational();
    for(;;){
        if(consume("==")) cur = new_node(ND_EQ, cur, relational());
        else if(consume("!=")) cur = new_node(ND_NE, cur, relational());
        else return cur;
    }
}

Node *relational(){
    Node *cur = add();
    for(;;){
        if(consume("<")) cur = new_node(ND_LT, cur, add());
        else if(consume("<=")) cur = new_node(ND_LE, cur, add());
        else if(consume(">")) cur = new_node(ND_LT, add(), cur);
        else if(consume(">=")) cur = new_node(ND_LE, add(), cur);
        else return cur;
    }
}

Node *add(){
    Node *cur = mul();
    for(;;){
        if(consume("+")) cur = new_node(ND_ADD, cur, mul());
        else if(consume("-")) cur = new_node(ND_SUB, cur, mul());
        else return cur;
    }
}

Node *mul(){
    Node *node = unary();
    for(;;){
        if(consume("*")) node = new_node(ND_MUL, node, unary());
        else if(consume("/")) node = new_node(ND_DIV, node, unary());
        else return node;
    }
}
Node *unary(){
    if(consume("+")) return unary();
    if(consume("-")) return new_node(ND_SUB,new_node_num(0),unary());
    return term();
}

Node *term(){
    if(consume("(")){
        Node *cur = expr();
        expect(")");
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
        case ND_EQ:
            printf("  cmp rax, rdi\n");
            printf("  sete al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_NE:
            printf("  cmp rax, rdi\n");
            printf("  setne al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LT:
            printf("  cmp rax, rdi\n");
            printf("  setl al\n");
            printf("  movzb rax, al\n");
            break;
        case ND_LE:
            printf("  cmp rax, rdi\n");
            printf("  setle al\n");
            printf("  movzb rax, al\n");
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