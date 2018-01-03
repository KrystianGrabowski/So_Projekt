#include <iostream>
#include <ucontext.h>
#include <queue>
#include <functional>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

ucontext_t mainf, new_context, new_context2;

void example_f1(){
    std::cout << "func1" << std::endl;
    sleep(3);
    swapcontext(&new_context, &mainf);
}
void example_f2(){
    std::cout << "func2" << std::endl;
}
void example_f3(){
    std::cout << "func3" << std::endl;
}

int thread_create(void (*f) ()){
    getcontext(&new_context);
    new_context.uc_link = 0;
    new_context.uc_stack.ss_sp = malloc(MEMORI);
    new_context.uc_stack.ss_size = MEMORI;
    new_context.uc_stack.ss_flags = 0;

    makecontext(&new_context, f, 0);
    return 0;
}

int main(){

    thread_create(&example_f1);
    swapcontext(&mainf, &new_context);


    std::cout << "Control_bk" << std::endl;
    sleep(3);

    return 0;
}
