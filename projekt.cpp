#include <iostream>
#include <ucontext.h>
#include <vector>
#include <unistd.h>

//#include "projekt.h"
#define MEMORI 16384

struct my_thread{
    ucontext_t context;
    int end;
    int id;
    int lock;
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
        thread0.id = -1;
        thread0.lock = 0;
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
        while((threads[curr].end == 1 || threads[curr].lock == 1) && curr != old ){
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

int thread_create(void (*f) (), int id_t){

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
    thread0.id = id_t;
    thread0.lock = 0;
    threads.push_back(thread0);

    return 0;

}
int find_in_vector(int id_t){
    int i = 0;
    while(i < threads.size()){
        if (threads[i].id == id_t){
            return i;
        }
        i++;
    }
    return -1;
}

int join(int id){
    int i = find_in_vector(id);
    if (i != -1){
        while(threads[i].end != 1){
            schedule();
        }
    }
    return 0;
}

int thread_wait(int id){

    int i = find_in_vector(id);
    if (i != -1){
        threads[i].lock = 1;
    }
    return 0;

}
int thread_signal(int id){

    int i = find_in_vector(id);
    if (i != -1){
        threads[i].lock = 0;
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
        if (i == 900){
            thread_wait(1);
        }
        if (i == 950){
            thread_signal(1);
        }
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

    thread_create(&funct_test4, 1);
    thread_create(&funct_test2, 2);
    thread_create(&funct_test3, 3);

    printf("Main control\n");
    join(1);
    join(2);
    join(3);
    printf("Main end\n");
    return 0;
}
