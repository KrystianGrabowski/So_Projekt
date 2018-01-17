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
bool main_thread_ex = false;
bool init_thread = false;
int size = 0;
int curr_thread = 0;
std::vector <my_thread> threads;



void schedule(){
    printf("SHR2\n");
    if (!main_thread_ex){

        threads.push_back(maincontext);
        main_thread_ex = true;
        swapcontext(&threads.back().context, &threads[curr_thread].context);


    }
    else{
        int i,j;
        if (curr_thread + 1 < threads.size()){
            i = curr_thread + 1;
            j = curr_thread;
        }
        else{
            i = 0;
            j = curr_thread;
        }

        while(threads[i].end == 1 && i != j){
        i++;

            if (i >= threads.size()){
                i = 0;
            }

        }

        curr_thread = i;
        printf("SHR3 %d %d\n", curr_thread, j);
        swapcontext(&threads[j].context, &threads[curr_thread].context);
    }
}

void done_f(){

    printf("Finisher\n");
    threads[curr_thread].end = 1;
    schedule();

}

int thread_create(void (*f) ()){

    if (!init_thread){

        getcontext(&finisher);
        finisher.uc_link = NULL;
        finisher.uc_stack.ss_sp = malloc(MEMORI);
        finisher.uc_stack.ss_size = MEMORI;
        finisher.uc_stack.ss_flags = 0;
        makecontext(&finisher, done_f, 0);
        init_thread = true;
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
    size++;
    return 0;

}


int thread_join(int number){

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
        printf("SHR\n");
        schedule();
    }
    printf("FUNC3 end");
    schedule();
}


int main(){

    thread_create(&funct_test1);
    thread_create(&funct_test2);
    thread_create(&funct_test3);
    schedule();
    printf("Main control\n");
    //threads.pop_back();
    thread_join(0);
    //thread_join(1);
    //thread_join(2);
    printf("Main end\n");
    return 0;
}
