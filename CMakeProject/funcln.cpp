
#include "pch.h"
#include <cmath>
#include <iostream>

extern "C" __declspec(dllexport)
double calculateLn(double x) {
    if (x > 0)
    {
        return log(x);
    }
    else
        std::cout << "  Error: Invalid expression under logarithm\n  ";
}
