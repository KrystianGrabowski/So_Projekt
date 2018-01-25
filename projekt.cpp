#include <iostream>
#include <ucontext.h>
#include <vector>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <queue>
#include "projekt.h"
#include <stdlib.h>
#include <cstdlib>

int mysliwi, kucharze, zwierzyna, pozywienie;
int ocalali;

my_semaphore semaphore;
my_semaphore lock1, lock2;

void schedule(){

    if (clock() - start > threads[curr].time_ft or threads[curr].end == 1 or threads[curr].lock == 1){
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

int my_sem_init(int value, my_semaphore *semaphore1){

    semaphore1->SEM = value;
    return 0;

}

int my_sem_wait(my_semaphore *semaphore1){

    semaphore1->SEM--;
    if(semaphore1->SEM < 0){
        semaphore1->queue.push(threads[curr].id);
        thread_wait(threads[curr].id);
        //schedule();

    }
    return 0;
}

int my_sem_signal(my_semaphore *semaphore1){

    semaphore1->SEM++;
    if(semaphore1->queue.empty() == false){
        int i = semaphore1->queue.front();
        thread_signal(i);
        semaphore1->queue.pop();

    }
    return 0;
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
    schedule();

    my_sem_wait(&semaphore);

    schedule();
    printf("CRITICAL12\n");
    schedule();
    printf("CRITICAL13\n");
    schedule();
    printf("CRITICAL14\n");
    schedule();
    printf("CRITICAL15\n");
    schedule();
    printf("CRITICAL16s\n");
    schedule();

    my_sem_signal(&semaphore);

    schedule();
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

///###################SETTLERS##############
int kostka_rzut(){
    return (std::rand() % 6) + 1;
}


void polowanie(){

    int mysliwy_rzut = kostka_rzut();
    schedule();
    int zwierz_rzut = kostka_rzut();
    schedule();
    my_sem_wait(&lock1);
    schedule();
    if (mysliwy_rzut > zwierz_rzut) {
        schedule();
        zwierzyna++;
        schedule();
        printf("Udane polowanie (Z) +1 <zwierzyna {%d}>\n", zwierzyna);
        schedule();
    }
    my_sem_signal(&lock1);
    schedule();


}

void pieczenie(){

    my_sem_wait(&lock1);
    schedule();
    if (zwierzyna > 0){
        schedule();
        zwierzyna--;
        schedule();
        my_sem_signal(&lock1);
        schedule();
        int kucharz_rzut = kostka_rzut();
        schedule();
        my_sem_wait(&lock1);
        schedule();
        pozywienie += kucharz_rzut;
        schedule();
        printf("Udane pieczenie (P) +%d <pozywienie {%d}>\n", kucharz_rzut, pozywienie);
        schedule();
    }
    my_sem_signal(&lock1);
    schedule();
}

void mysliwy(){
    for (int dzien = 1; dzien <= 365; dzien++){

        polowanie();
        schedule();
        my_sem_wait(&lock1);
        schedule();
        if (pozywienie > 0){
            schedule();
            pozywienie--;
            schedule();
            my_sem_signal(&lock1);
            schedule();
            usleep(500);
            schedule();
        }
        else{

            printf("Mysliwy odszedl z wioski !!\n");
            schedule();
            ocalali--;
            schedule();
            my_sem_signal(&lock1);
            schedule();
            break;

        }

    }
}

void kucharz(){
    for (int dzien = 1; dzien <= 365; dzien++){

        pieczenie();
        schedule();
        my_sem_wait(&lock1);
        schedule();
        if (pozywienie > 0 ){
            schedule();
            pozywienie--;
            schedule();
            my_sem_signal(&lock1);
            schedule();
            usleep(500);
            schedule();
        }
        else{
            printf("Kucharz odszedl z wioski !!\n");
            schedule();
            ocalali--;
            schedule();
            my_sem_signal(&lock1);
            schedule();
            break;
            schedule();
        }

    }
}

int osada(){

    int k, m;
    srand( time( NULL ) );
    my_sem_init(1, &lock1);
    my_sem_init(1, &lock2);
    mysliwi = 10;
    kucharze = 10;
    zwierzyna = 500;
    pozywienie = 500;
    ocalali = mysliwi + kucharze;

    int tab_k[kucharze];
    int tab_m[mysliwi];

    for (k = 0; k < kucharze; k++){
        thread_create(&kucharz, k, 10);
    }
    for (m = 0; m < mysliwi; m++){
        thread_create(&mysliwy, m + kucharze, 10);
    }

    for (k = 0; k < kucharze; k++){
        join(k);
    }
    for (m = 0; m < mysliwi; m++){
        join(m+kucharze);
    }
    printf("\nPodsumowanie :\n ocalali = %d \n pozywnie = %d\n", ocalali, pozywienie);

    return 0;
}
// ********************************
int main(){

    /*thread_create(&funct_test4, 1, 2);
    thread_create(&funct_test2, 2, 2);
    thread_create(&funct_test3, 3, 2);*/    //TEST1

    /*thread_create(&semtest1, 1, 10);
    thread_create(&semtest1, 2, 10);
    thread_create(&semtest1, 3, 10);
    my_sem_init(1, &semaphore);  */   //TEST2


    osada();
    printf("Main control\n");
    join(1);
    join(2);
    //join(3);
    printf("Main end\n");
    return 0;
}
