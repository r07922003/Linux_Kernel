#include "9cc.h"

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

void expect(Token** token,char *op){
    if((*token)->kind != TK_RESERVED || strlen(op)!= (*token)->len || memcmp((*token)->str, op, (*token)->len)) error_at((*token)->str,"not is %c",op);
    *token = (*token)->next;
}

int expect_number(Token **token){
    if((*token)->kind != TK_NUM) error_at((*token)->str,"Not a number");
    int val = (*token)->val;
    *token = (*token)->next;
    return val;
}

bool is_alpha(char c){
    return ('a'<=c && c<='z') || ('A'<=c && c<='Z') || c=='_';
}

bool is_alnum(char c){
    return is_alpha(c) || ('0'<=c && c<='9');
}

bool consume(Token **token,char *op){
    if((*token)->kind != TK_RESERVED || strlen(op)!= (*token)->len || memcmp((*token)->str, op, (*token)->len)) return false;
    *token = (*token)->next;
    return true;
}

Token *consume_ident(Token **token){
    if((*token)->kind != TK_IDENT) return NULL;
    Token *res = *token;
    *token = (*token)->next;
    return res;
}

bool startswith(char *a, char *b){
    return memcmp(a,b,strlen(b)) == 0;
}

bool at_eof(Token *token){
    return token->kind == TK_EOF;
}

Token *__new_token(TokenKind tokenKind, Token *cur, char *str,int len){
    Token *token = calloc(1,sizeof(Token));
    token->kind = tokenKind;
    token->str = str;
    token->len = len;
    cur->next = token;
    return token;
}

Token *__tokenize(char *p){
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while(*p){
        if(isspace(*p)){
            p++;
            continue;;
        }
        if(startswith(p,"return") && !is_alnum(p[6])){
            cur = __new_token(TK_RESERVED, cur, p,6);
            p+=6;
            continue;
        }
        if(startswith(p,"==") || startswith(p,"!=") || startswith(p,"<=") || startswith(p,">=")){
            cur = __new_token(TK_RESERVED, cur, p,2);
            p+=2;
            continue;
        }
        if(strchr("+-*/()<>;=", *p)){
            cur = __new_token(TK_RESERVED, cur, p++,1);
            continue;
        }
        if('a'<=*p && *p<='z'){
            cur = __new_token(TK_IDENT, cur, p++, 1);
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