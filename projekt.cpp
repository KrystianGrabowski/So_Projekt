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
int curr = 0;
bool FINISHER_INIT = false;
bool MAIN_INIT = false;
std::vector <my_thread> threads;



void schedule(){
    if (MAIN_INIT == false){

        my_thread thread0;
        thread0.context.uc_link = 0;
        thread0.context.uc_stack.ss_sp = malloc(MEMORI);
        thread0.context.uc_stack.ss_size = MEMORI;
        thread0.context.uc_stack.ss_flags = 0;
        thread0.end = 0;
        threads.push_back(thread0);
        MAIN_INIT = true;
        swapcontext(&threads.back().context, &threads[curr].context);
    }

    else{

        int old;
        old = curr;

        if (curr < threads.size() - 1){
            curr = curr + 1;
        }
        else{
            curr = 0;
        }

        while(threads[curr].end == 1 && curr != old){
            curr++;
            if (curr >= threads.size()){
                curr = 0;
            }
        }
        swapcontext(&threads[old].context, &threads[curr].context);
    }

}

void done_f(){

    threads[curr].end = 1;
    schedule();

}

int thread_create(void (*f) ()){

    if (FINISHER_INIT == false){

        getcontext(&finisher);
        finisher.uc_link = 0;
        finisher.uc_stack.ss_sp = malloc(MEMORI);
        finisher.uc_stack.ss_size = MEMORI;
        finisher.uc_stack.ss_flags = 0;
        makecontext(&finisher, done_f, 0);
        FINISHER_INIT = true;

    }
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

int join(int number){

    while(threads[number].end != 1){
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

void funct_test4(){
    for (int i = 0; i < 1000; i++){
        printf("Funckja 4 %d\n", i);
        schedule();
    }
    printf("FUNC4 end");
    schedule();
}

int main(){

    thread_create(&funct_test4);
    thread_create(&funct_test2);
    thread_create(&funct_test3);

    printf("Main control\n");
    join(0);
    join(1);
    join(2);
    printf("Main end\n");
    return 0;
}
