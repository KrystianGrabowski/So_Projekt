#include <iostream>
#include <ucontext.h>
#include <queue>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

ucontext_t mainf, new_context, new_context2;
struct my_thread{
    ucontext_t context;
};

std::queue <my_thread> threads;

void example_f1(){
    printf("func1\n");
    swapcontext(&new_context, &new_context2);
    printf("endf1\n");
}
void example_f2(){

    printf("func2\n");
    swapcontext(&new_context2, &new_context);
    printf("endf2\n");
}

int thread_create(void (*f) ()){

    my_thread thread0;
    getcontext(&thread0.context);
    thread0.context.uc_link = &mainf;
    thread0.context.uc_stack.ss_sp = malloc(MEMORI);
    thread0.context.uc_stack.ss_size = MEMORI;
    thread0.context.uc_stack.ss_flags = 0;
    makecontext(&new_context, f, 0);
    threads.push(thread0);

    return 0;

}

void schedule(){

}

int join(){
    return 0;
}


int main(){

    thread_create(&example_f1);
    thread_create(&example_f2);
    printf("CTRL");
    return 0;
}
