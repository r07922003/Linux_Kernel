#include "9cc.h"

char *user_input;

int main(int argc,char **argv){
    if(argc != 2){
        error("The numbers of arguments are wrong");
        return  1;
    }

    user_input = argv[1];
    Token *token = __tokenize(user_input);
    Node *cur_node = expr(&token);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    gen(cur_node);

    printf("  pop rax\n");
    printf("  ret\n");
    return 0;
}