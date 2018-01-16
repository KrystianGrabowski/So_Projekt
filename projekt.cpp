#include <iostream>
#include <ucontext.h>
#include <vector>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

struct my_thread{
    ucontext_t context;
    int end;
};
ucontext_t finisher;
my_thread maincontext;
int size = 0;
std::vector <my_thread> threads;

int init_thread_create(void (*f) ()){
    printf("ASD");

    getcontext(&finisher);


    finisher.uc_link = NULL;
    finisher.uc_stack.ss_sp = malloc(MEMORI);
    finisher.uc_stack.ss_size = MEMORI;
    finisher.uc_stack.ss_flags = 0;
    makecontext(&finisher, f, 0);
    return 0;
}

int thread_create(void (*f) ()){

    my_thread thread0;
    getcontext(&thread0.context);
    thread0.context.uc_link = &finisher;
    thread0.context.uc_stack.ss_sp = malloc(MEMORI);
    thread0.context.uc_stack.ss_size = MEMORI;
    thread0.context.uc_stack.ss_flags = 0;
    makecontext(&thread0.context, f, 0);
    threads.push_back(thread0);

    return 0;

}

void schedule(){

    size++;
    size = size % threads.size();

    if(threads[size].end == 0){
        if (size == 0){
            swapcontext(&threads[threads.size() -1].context, &threads[0].context);
        }
        else{
            swapcontext(&threads[size - 1].context, &threads[size].context);
        }
    }
    else{
        schedule();
    }


}

int join(){

    return 0;
}

void done_f(){
    printf("DSDADA");
    threads[size].end = 1;
    schedule();
}


void funct_test1(){
    printf("print_test1");
    schedule();

}
void funct_test2(){
    printf("print_test4");
    schedule();
    printf("print_test5");
    schedule();
    printf("print_test6");
    schedule();
}

int main(){
    printf("ASD");

    init_thread_create(&done_f);
    thread_create(&funct_test1);
    thread_create(&funct_test2);
    //threads.push_back(maincontext);
    swapcontext(&maincontext.context, &threads[size].context); //&threads.back().context

    printf("Main control");
    return 0;
}
