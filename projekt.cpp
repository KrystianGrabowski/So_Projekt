#include <iostream>
//#include <ucontext.h>
#include <queue>
#include <functional>
//#include "projekt.h"


std::queue<std::function<void()>> functions;


void example_f1(){
    std::cout << "func1" << std::endl;
}
void example_f2(){
    std::cout << "func2" << std::endl;
}
void example_f3(){
    std::cout << "func3" << std::endl;
}

void acc( void (*f) () ){
    functions.push(*f);
}

void exec(){
    functions.front()();
    functions.pop();
    functions.front()();
    functions.pop();
    functions.front()();

}

int main(){
    acc(&example_f1);
    acc(&example_f2);
    acc(&example_f3);
    exec();
    return 0;
}
