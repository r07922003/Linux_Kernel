#include <stdio.h>
#include <stdlib.h>

typedef struct object Object;
typedef int (*func_t)(Object *);

struct object{
    int a,b;
    func_t add, sub;
};

int init_object(Object **this,int a,int b);