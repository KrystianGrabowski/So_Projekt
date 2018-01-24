#include <iostream>
#include <ucontext.h>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <queue>


//#include "projekt.h"
#define MEMORI 16384

struct my_thread{
    ucontext_t context;
    int end;
    int id;
    int lock;
    int time_ft;
};

int start = clock();
ucontext_t finisher;
my_thread maincontext;
int curr = 0;
bool FINISHER_INIT = false;
bool MAIN_INIT = false;
bool TIMER_INIT = false;
std::vector <my_thread> threads;

//SEMAPHORE

struct my_semaphore{
    int SEM;
    std::queue <int> queue;
};
my_semaphore semaphore;


void schedule(){

    if (clock() - start > threads[curr].time_ft or threads[curr].end == 1){
        start = clock();
        if (MAIN_INIT == false){

            my_thread thread0;
            thread0.context.uc_link = 0;
            thread0.context.uc_stack.ss_sp = malloc(MEMORI);
            thread0.context.uc_stack.ss_size = MEMORI;
            thread0.context.uc_stack.ss_flags = 0;
            thread0.end = 0;
            thread0.id = -1;
            thread0.lock = 0;
            thread0.time_ft = 10;
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

}

void finish(){
    threads[curr].end = 1;
    schedule();

}

int thread_create(void (*f) (), int id_t, int time_for_thread){

    if (FINISHER_INIT == false){

        getcontext(&finisher);
        finisher.uc_link = 0;
        finisher.uc_stack.ss_sp = malloc(MEMORI);
        finisher.uc_stack.ss_size = MEMORI;
        finisher.uc_stack.ss_flags = 0;
        makecontext(&finisher, finish, 0);
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
    thread0.time_ft = time_for_thread;
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
    printf("FUN 1.1\n");
    schedule();
    printf("FUN1 end\n");
    schedule();
}

void funct_test2(){
    printf("FUN 2.1\n");
    schedule();
    printf("FUN 2.1\n");
    schedule();
    printf("FUN2 end\n");
    schedule();
}

void funct_test3(){
    for (int i = 0; i < 500; i++){
        /*if (i == 900){
            thread_wait(1);
        }
        if (i == 950){
            thread_signal(1);
        }*/
        printf("FUN 3. %d\n", i);
        schedule();
    }
    printf("FUN3 end\n");
    schedule();
}

void funct_test4(){
    for (int i = 0; i < 500; i++){
        printf("FUN 4 %d\n", i);
        schedule();
    }
    printf("FUN4 end\n");
    schedule();
}
//###################SEMAPHORE#####################

int my_sem_init(int value, my_semaphore s){
    s.SEM = value;
}

int my_sem_wait(my_semaphore sem1){
    sem1.SEM--;
    if (sem1.SEM < 0) {
        sem1.queue.push(threads[curr].id);
        thread_wait(threads[curr].id);
        schedule();
    }

}

int my_sem_signal(my_semaphore sem1){
    printf("TEST");
    sem1.SEM++;
    if (!sem1.queue.empty()){
        int i = sem1.queue.front();
        i = find_in_vector(i);
        sem1.queue.pop();
        thread_signal(i);
        printf("dd");
    }
    //schedule();
}


void semtest1(){
    printf("TEST1\n");
    schedule();
    printf("TEST2\n");
    schedule();
    printf("TEST3\n");
    schedule();
    printf("TEST4\n");
    schedule();
    printf("TEST5\n");

    //my_sem_wait(semaphore);
    printf("CRITICAL12\n");
    schedule();
    printf("CRITICAL13\n");
    schedule();
    printf("CRITICAL14\n");
    schedule();
    printf("CRITICAL15\n");
    schedule();
    printf("CRITICAL16s\n");

    //my_sem_signal(semaphore);

    printf("TEST6\n");
    schedule();
    printf("TEST7\n");
    schedule();
    printf("TEST8\n");
    schedule();
    printf("TEST9\n");
    schedule();
    printf("TEST10\n");
    schedule();


}
int main(){

    /*thread_create(&funct_test4, 1, 2);
    thread_create(&funct_test2, 2, 2);
    thread_create(&funct_test3, 3, 2);*/

    thread_create(&semtest1, 1, 10);
    thread_create(&semtest1, 2, 10);

    my_sem_init(1, semaphore);
    std::cout << semaphore.queue.size() << std::endl;

    printf("Main control\n");
    join(1);
    /*join(2);
    join(3);*/
    printf("Main end\n");
    return 0;
}
