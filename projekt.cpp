#include <iostream>
#include <ucontext.h>
#include <vector>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

struct my_thread{
    ucontext_t context;
};
my_thread maincontext, thread_init;
int size = 0;
std::vector <my_thread> threads;

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
    threads.push_back(thread0);

    return 0;

}

void schedule(){

    size++;
    if (size == threads.size()){
        size = 0;
        swapcontext(&threads[threads.size() -1].context, &threads[0].context);
    }
    else{
        swapcontext(&threads[size - 1].context, &threads[size].context);
    }

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
void funct_test2(){
    printf("print_test4\n");
    schedule();
    printf("print_test5\n");
    schedule();
    printf("print_test6\n");
    schedule();
}

int main(){
    thread_create(&funct_test1);
    thread_create(&funct_test2);
    swapcontext(&maincontext.context, &threads[size].context);
    printf("Main control");
    return 0;
}
