#include "9cc.h"

char *user_input;

int main(int argc,char **argv){
    if(argc != 2){
        error("The numbers of arguments are wrong");
        return  1;
    }

    user_input = argv[1];
    Token *token = __tokenize(user_input);
    Program *cur_prog = program(&token);

    // Asign offsets to local variables
    int offset = 0;
    for(Var *cur_var = cur_prog->locals; cur_var; cur_var=cur_var->next){
        offset += 8;
        cur_var->offset = offset;
    }
    cur_prog->stack_size = offset;

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // Prologue
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n",cur_prog->stack_size);

    for(Node *cur=cur_prog->node;cur;cur = cur->next){
        gen(cur);
    }

    // Epilogue
    printf(".Lreturn:\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;
}