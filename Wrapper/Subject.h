#pragma once
#include <iostream>

class Subject {
public:
    // 2 functions with different return types
    int f1(int a, int b) {
        std::cout << "Function f1 is called. Its arguments: " << a << ", " << b << std::endl;
        return a + b;
    };
    std::string f2(const std::string& str1, const std::string& str2) {
        std::cout << "Function f2 is called. Its arguments: " << str1 << ", " << str2 << std::endl;
        return str1 + str2;
    };
};