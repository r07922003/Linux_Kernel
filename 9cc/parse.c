#include "9cc.h"

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


/*
 expr = equality
 equality = relational ("==" relational | "!=" relational)*
 relational = add ("<" add | "<=" add | ">" add | ">=" add)*
 add = mul ('+' mul | '-' mul) *
 mul = unary ('*' unary | '/' unary) *
 unary = ("+" unary | "-" unary)? term
 term = num | '(' expr ')'
*/

Node *expr(Token** token){
    return equality(token);
}

Node *equality(Token** token){
    Node *cur = relational(token);
    for(;;){
        if(consume(token,"==")) cur = new_node(ND_EQ, cur, relational(token));
        else if(consume(token,"!=")) cur = new_node(ND_NE, cur, relational(token));
        else return cur;
    }
}

Node *relational(Token** token){
    Node *cur = add(token);
    for(;;){
        if(consume(token,"<")) cur = new_node(ND_LT, cur, add(token));
        else if(consume(token,"<=")) cur = new_node(ND_LE, cur, add(token));
        else if(consume(token,">")) cur = new_node(ND_LT, add(token), cur);
        else if(consume(token,">=")) cur = new_node(ND_LE, add(token), cur);
        else return cur;
    }
}

Node *add(Token** token){
    Node *cur = mul(token);
    for(;;){
        if(consume(token,"+")) cur = new_node(ND_ADD, cur, mul(token));
        else if(consume(token,"-")) cur = new_node(ND_SUB, cur, mul(token));
        else return cur;
    }
}

Node *mul(Token** token){
    Node *node = unary(token);
    for(;;){
        if(consume(token,"*")) node = new_node(ND_MUL, node, unary(token));
        else if(consume(token,"/")) node = new_node(ND_DIV, node, unary(token));
        else return node;
    }
}

Node *unary(Token** token){
    if(consume(token,"+")) return unary(token);
    if(consume(token,"-")) return new_node(ND_SUB,new_node_num(0),unary(token));
    return term(token);
}

Node *term(Token** token){
    if(consume(token,"(")){
        Node *cur = expr(token);
        expect(token,")");
        return cur;
    }
    return new_node_num(expect_number(token));
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