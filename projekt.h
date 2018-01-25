#include <iostream>

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


void schedule();
void finish();
int thread_create(void (*f) (), int id_t, int time_for_thread);
int find_in_vector(int id_t);
int join(int id);
int thread_wait(int id);
int thread_signal(int id);

int my_sem_init(int value, my_semaphore *semaphore1);
int my_sem_wait(my_semaphore *semaphore1);
int my_sem_signal(my_semaphore *semaphore1);
