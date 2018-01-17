#include <iostream>

int thread_create(void (*f) ());
void done_f();
void schedule();
int thread_join(int number);
