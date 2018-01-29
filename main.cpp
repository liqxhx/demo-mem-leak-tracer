#include <iostream>
#include "DebugNew.h"

int main() {
    std::cout << "Hello, mem-tracer!" << std::endl;

    int *p = new int;
    delete p;
    //std::cout<<""<<std::endl;
    return 0;
}