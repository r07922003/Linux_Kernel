#include "9cc.h"

char *user_input;

int main(int argc,char **argv){
    if(argc != 2){
        error("The numbers of arguments are wrong");
        return  1;
    }

    user_input = argv[1];
    Token *token = __tokenize(user_input);
    Node *cur_node = program(&token);

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    for(Node *cur=cur_node;cur;cur = cur->next){
        gen(cur);
    }

    printf("  ret\n");
    return 0;
}