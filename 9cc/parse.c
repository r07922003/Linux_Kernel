#include "9cc.h"

Var *locals;

// O(n) sequential way to find the duplicate local variable
Var *find_var(Token *token){
    for(Var *cur_var = locals; cur_var; cur_var=cur_var->next){
        if(strlen(cur_var->name)==token->len && !memcmp(token->str,cur_var->name,token->len)){
            return cur_var;
        }
    }
    return NULL;
}

Node *new_node(NodeKind kind){
    Node *cur = calloc(1, sizeof(Node));
    cur->kind = kind;
    return cur;
}

Node *new_node_binary(NodeKind kind,Node *l,Node *r){
    Node *cur = calloc(1, sizeof(Node));
    cur->kind = kind;
    cur->l = l;
    cur->r = r;
    return cur;
}

Node *new_node_unary(NodeKind kind,Node *expr){
    Node *cur = new_node(kind);
    cur->l = expr;
    return cur;
}

Node *new_node_num(int val){
    Node *cur = calloc(1,sizeof(Node));
    cur->kind = ND_NUM;
    cur->val = val;
    return cur;
}

Node *new_node_var(Var *cur_var){
    Node *cur = new_node(ND_VAR);
    cur->var = cur_var;
    return cur;
}

Var *push_var(char *name){
    Var *cur_var =  calloc(1,sizeof(Var));
    cur_var->next = locals;
    cur_var->name = name;
    locals = cur_var;
    return cur_var;
}

/*
 program = stmt*
 stmt = return expr ";" | expr ";"
 expr = assign
 assign = equality ("=" assign)?
 equality = relational ("==" relational | "!=" relational)*
 relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 add = mul ('+' mul | '-' mul) *
 mul = unary ('*' unary | '/' unary) *
 unary = ("+" unary | "-" unary)? primary
 primary = "(" expr ")" | ident | num
*/
Program *program(Token ** token){
    locals = NULL;

    Node head;
    head.next = NULL;
    Node *cur = &head;

    while(!at_eof(*token)){
        cur->next = stmt(token);
        cur = cur->next;
    }

    Program *cur_prog = calloc(1,sizeof(Program));
    cur_prog->node = head.next;
    cur_prog->locals = locals;
    return cur_prog;
}

Node *stmt(Token **token){
    if(consume(token,"return")){
        Node *cur = new_node_unary(ND_RETURN, expr(token));
        expect(token,";");
        return cur;
    }
    Node *cur = new_node_unary(ND_EXPR_STMT, expr(token));
    expect(token,";");
    return cur;
}

Node *expr(Token** token){
    return assign(token);
}

Node *assign(Token **token){
    Node *cur = equality(token);
    if(consume(token,"=")) cur = new_node_binary(ND_ASSIGN, cur, assign(token));
    return cur;
}

Node *equality(Token** token){
    Node *cur = relational(token);
    for(;;){
        if(consume(token,"==")) cur = new_node_binary(ND_EQ, cur, relational(token));
        else if(consume(token,"!=")) cur = new_node_binary(ND_NE, cur, relational(token));
        else return cur;
    }
}

Node *relational(Token** token){
    Node *cur = add(token);
    for(;;){
        if(consume(token,"<")) cur = new_node_binary(ND_LT, cur, add(token));
        else if(consume(token,"<=")) cur = new_node_binary(ND_LE, cur, add(token));
        else if(consume(token,">")) cur = new_node_binary(ND_LT, add(token), cur);
        else if(consume(token,">=")) cur = new_node_binary(ND_LE, add(token), cur);
        else return cur;
    }
}

Node *add(Token** token){
    Node *cur = mul(token);
    for(;;){
        if(consume(token,"+")) cur = new_node_binary(ND_ADD, cur, mul(token));
        else if(consume(token,"-")) cur = new_node_binary(ND_SUB, cur, mul(token));
        else return cur;
    }
}

Node *mul(Token** token){
    Node *node = unary(token);
    for(;;){
        if(consume(token,"*")) node = new_node_binary(ND_MUL, node, unary(token));
        else if(consume(token,"/")) node = new_node_binary(ND_DIV, node, unary(token));
        else return node;
    }
}

Node *unary(Token** token){
    if(consume(token,"+")) return unary(token);
    if(consume(token,"-")) return new_node_binary(ND_SUB,new_node_num(0),unary(token));
    return primary(token);
}

Node *primary(Token** token){
    if(consume(token,"(")){
        Node *cur = expr(token);
        expect(token,")");
        return cur;
    }

    Token *temp = consume_ident(token);
    if(temp){
        Var *cur_var = find_var(temp);
        if(!cur_var) cur_var = push_var(strndup(temp->str,temp->len));
        return new_node_var(cur_var);
    }

    return new_node_num(expect_number(token));
}