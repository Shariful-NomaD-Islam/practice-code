#include "example.h"
#include <iostream>
#include <string>

extern "C"
{
    int add(int a, int b)
    {
        return a + b;
    }

    int multiply(int a, int b)
    {
        return a * b;
    }

    void print_hello(const char *name)
    {
        std::cout << "Hello from C++, " << std::string(name) << "!!!" << std::endl;
    }
}
