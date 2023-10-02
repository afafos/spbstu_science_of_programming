#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cctype>
#include <windows.h>
#include "struct.h"

int main() {

    HINSTANCE hDLL = LoadLibrary("plugins\\funcsin.dll"); // Loading a DLL File
    if (hDLL == NULL) {
        std::cerr << "Error loading DLL" << std::endl;
        return 1;
    }

    calculateSin = (MathFunction)GetProcAddress(hDLL, "calculateSin"); 
    if (calculateSin == NULL) {
        std::cerr << "Error loading function" << std::endl;
        return 1;
    }


    hDLL = LoadLibrary("plugins\\funccos.dll"); 
    if (hDLL == NULL) {
        std::cerr << "Error loading DLL" << std::endl;
        return 1;
    }

    calculateCos = (MathFunction)GetProcAddress(hDLL, "calculateCos"); 
    if (calculateCos == NULL) {
        std::cerr << "Error loading function" << std::endl;
        return 1;
    }


    hDLL = LoadLibrary("plugins\\functan.dll"); 
    if (hDLL == NULL) {
        std::cerr << "Error loading DLL" << std::endl;
        return 1;
    }

    calculateTan = (MathFunction)GetProcAddress(hDLL, "calculateTan"); 
    if (calculateCos == NULL) {
        std::cerr << "Error loading function" << std::endl;
        return 1;
    }


    hDLL = LoadLibrary("plugins\\funcln.dll"); 
    if (hDLL == NULL) {
        std::cerr << "Error loading DLL" << std::endl;
        return 1;
    }

    calculateLn = (MathFunction)GetProcAddress(hDLL, "calculateLn"); 
    if (calculateCos == NULL) {
        std::cerr << "Error loading function" << std::endl;
        return 1;
    }


    hDLL = LoadLibrary("plugins\\funcpower.dll"); 
    if (hDLL == NULL) {
        std::cerr << "Error loading DLL" << std::endl;
        return 1;
    }

    calculatePower = (MathFunction1)GetProcAddress(hDLL, "calculatePower"); 
    if (calculatePower == NULL) {
        std::cerr << "Error loading function" << std::endl;
        return 1;
    }


    Calculator calculator;
    std::string expression;

    std::cout << "---- The calculator supports operations - + * / ^ sin cos tan ln ----\n  ";
    std::cout << "Enter a mathematical expression: ";
    std::getline(std::cin, expression);

    try {
        double result = calculator.evaluate(expression);
        std::cout << "  Calculation result: " << result << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "    Error: " << e.what() << std::endl;
    }

    return 0;
}