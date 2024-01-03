#include "9cc.h"

void gen_addr(Node *node){
  if (node->kind == ND_VAR) {
    printf("  lea rax, [rbp-%d]\n", node->var->offset);
    printf("  push rax\n");
    return;
  }

  error("not an lvalue");
}

void load(){
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
}

void store(){
    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
}

void gen(Node *node){
    switch(node->kind){
        case ND_NUM:
            printf("  push %d\n",node->val);
            return;
        case ND_EXPR_STMT:
            gen(node->l);
            printf("  add rsp, 8\n");
            return;
        case ND_VAR:
            gen_addr(node);
            load();
            return;
        case ND_ASSIGN:
            gen_addr(node->l);
            gen(node->r);
            store();
            return;
        case ND_RETURN:
            gen(node->l);
            printf("  pop rax\n");
            printf("  jmp .Lreturn\n");
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