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

int thread_create(void (*f) ()){

    my_thread thread0;
    getcontext(&thread0.context);
    thread0.context.uc_link = &finisher;
    thread0.context.uc_stack.ss_sp = malloc(MEMORI);
    thread0.context.uc_stack.ss_size = MEMORI;
    thread0.context.uc_stack.ss_flags = 0;
    makecontext(&thread0.context, f, 0);
    thread0.end = 0;
    threads.push_back(thread0);

    return 0;

}

void schedule(){
    int i,j;
    if (size + 1 < threads.size()){
        i = size + 1;
        j = size;
    }
    else{
        i = 0;
        j = size;
    }

    while(threads[i].end == 1 && i != j){
        i++;
        if (i >= threads.size()){
            i = 0;
        }

    }
    size = i;
    swapcontext(&threads[j].context, &threads[size].context);

}

void done_f(){
    printf("Finisher\n");
    threads[size].end = 1;
    schedule();
}

int join(int number){

    while(threads[number].end == 0){
        schedule();
    }
    return 0;
}


void funct_test1(){
    printf("print_test1\n");
    schedule();
    printf("FUNC1 end\n");
    schedule();
}

void funct_test2(){
    printf("print_test4\n");
    schedule();
    printf("print_test5\n");
    schedule();
    printf("FUNC2 end\n");
    schedule();
}

void funct_test3(){
    for (int i = 0; i < 1000; i++){
        printf("Funckja 3 %d\n", i);
        schedule();
    }
    printf("FUNC3 end");
    schedule();
}


int main(){

    getcontext(&finisher);
    finisher.uc_link = 0;
    finisher.uc_stack.ss_sp = malloc(MEMORI);
    finisher.uc_stack.ss_size = MEMORI;
    finisher.uc_stack.ss_flags = 0;
    makecontext(&finisher, done_f, 0);

    thread_create(&funct_test1);
    thread_create(&funct_test2);
    thread_create(&funct_test3);

    threads.push_back(maincontext);
    swapcontext(&threads.back().context, &threads[size].context); //&threads.back().context

    printf("Main control\n");
    //join(0);
    //join(1);
    join(2);
    printf("Main end\n");
    return 0;
}
