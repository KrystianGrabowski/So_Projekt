#include <iostream>
#include <ucontext.h>
#include <queue>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

struct my_thread{
    ucontext_t context;
};
my_thread maincontext, thread_init;

std::queue <my_thread> threads;

void example_f1(){

    for (int i = 0; i < 100; i++){
        printf("b\n");
    }
}


int thread_create(void (*f) ()){

    my_thread thread0;
    getcontext(&thread0.context);
    thread0.context.uc_link = &maincontext.context;
    thread0.context.uc_stack.ss_sp = malloc(MEMORI);
    thread0.context.uc_stack.ss_size = MEMORI;
    thread0.context.uc_stack.ss_flags = 0;
    makecontext(&thread0.context, f, 0);
    threads.push(thread0);

    return 0;

}

void schedule(){

    my_thread threadsave, threadnext;


    threadnext = threads.front();
    threads.pop();

    getcontext(&threadsave.context);
    threads.push(threadsave);

    setcontext(&threadnext.context);


}

int join(){
    return 0;
}

void funct_test1(){
    printf("print_test1\n");
    schedule();
    printf("print_test2\n");
    schedule();
    printf("print_test3\n");
    schedule();
}
int main(){

    thread_create(&funct_test1);
    thread_create(&funct_test1);
    thread_init = threads.front();
    threads.pop();
    swapcontext(&maincontext.context, &thread_init.context);
    printf("Main control");
    return 0;
}
