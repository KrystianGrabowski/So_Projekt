#include <iostream>
#include <ucontext.h>
#include <queue>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

//std::queue <functionptr> threads;

ucontext_t mainf, new_context, new_context2;

void example_f1(){
    printf("func1\n");
    sleep(3);
    swapcontext(&new_context, &new_context2);
    printf("endf1\n");
}
void example_f2(){

    printf("func2\n");
    swapcontext(&new_context2, &new_context);
    printf("endf2\n");
}

int thread_create(void (*f) ()){
    getcontext(&new_context);
    new_context.uc_link = &mainf;
    new_context.uc_stack.ss_sp = malloc(MEMORI);
    new_context.uc_stack.ss_size = MEMORI;
    new_context.uc_stack.ss_flags = 0;

    makecontext(&new_context, f, 0);
    return 0;
}
int thread_create2(void (*f) ()){
    getcontext(&new_context2);
    new_context2.uc_link = &new_context;
    new_context2.uc_stack.ss_sp = malloc(MEMORI);
    new_context2.uc_stack.ss_size = MEMORI;
    new_context2.uc_stack.ss_flags = 0;

    makecontext(&new_context2, f, 0);
    printf("TEST\n");

    return 0;
}

void schedule(){

}

int join(){
    return 0;
}


int main(){

    thread_create(&example_f1);
    thread_create2(&example_f2);
    swapcontext(&mainf, &new_context2);
    printf("Control_bk\n");
    sleep(3);

    return 0;
}
