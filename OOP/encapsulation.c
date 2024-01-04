#include "encapsulation.h"

static int add_impl(Object *this){
    return this->a + this->b;
}

static int sub_impl(Object *this){
    return this->a - this->b;
}

int init_object(Object **this,int a,int b){
    if(NULL==(*this = malloc(sizeof(Object)))) return -1;
    (*this)->a = a;
    (*this)->b = b;
    (*this)->add = add_impl;
    (*this)->sub = sub_impl;
    return 0;
}