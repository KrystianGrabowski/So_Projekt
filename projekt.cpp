#include <iostream>
#include <ucontext.h>
#include <queue>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

ucontext_t maincontext;
struct my_thread{
    ucontext_t context;
};

std::queue <my_thread> threads;

void example_f1(){

    for (int i = 0; i < 100; i++){
        printf("b\n");
    }
}
void test1(){
    printf("print_test1")
    schedule()
    printf("print_test2")
    schedule()
    printf("print_test2")
    schedule()
}

int thread_create(void (*f) ()){

    my_thread thread0;
    getcontext(&thread0.context);
    thread0.context.uc_link = &maincontext;
    thread0.context.uc_stack.ss_sp = malloc(MEMORI);
    thread0.context.uc_stack.ss_size = MEMORI;
    thread0.context.uc_stack.ss_flags = 0;
    makecontext(&thread0.context, f, 0);
    threads.push(thread0);

    return 0;

}

void schedule(){

    my_thread thread1;
    thread1 = threads.front();
    swapcontext(&maincontext, &thread1.context);

}

int join(){
    return 0;
}


int main(){

    thread_create(&funct_test1);
    thread_create(&funct_test2);
    return 0;
}
