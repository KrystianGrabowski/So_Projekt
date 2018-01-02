#include <iostream>
#include <ucontext.h>
//#include "projekt.h"




void example_f(){
    std::cout << "func" << std::endl;
}

void myfunc( void (*f) () ){
    f();
}

int main(){
    myfunc(&example_f);
    return 0;
}
