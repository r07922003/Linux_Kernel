#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>

typedef enum{
    TK_RESERVED, //symbol
    TK_NUM,      //number
    TK_EOF,      //end symbol
} TokenKind;

typedef struct Token Token;

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
        if(*p=='+' || *p=='-'){
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


int main(int argc,char **argv){
    if(argc != 2){
        error("The numbers of arguments are wrong");
        return  1;
    }

    user_input = argv[1];
    token = __tokenize();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    printf("  mov rax, %d\n",expect_number());

    while(!at_eof(token)){
        if(consume('+'))
        {
            printf("  add rax, %d\n",expect_number());
            continue;
        }
        expect('-');
        printf("  sub rax, %d\n",expect_number());
    }

    printf("  ret\n");
    return 0;
}