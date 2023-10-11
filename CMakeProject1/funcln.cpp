#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <windows.h>
#include "pch.h"
#include <iostream>
using std::string;

enum class OperatorType {
    Unary,
    Binary
};

extern "C" {

    __declspec(dllexport) string sym = "ln";
    __declspec(dllexport) OperatorType un_or_bin = OperatorType::Unary;
    __declspec(dllexport) double eval(const std::vector<double> x) {
        double value = x[0];
        if (value <= 0)
            throw std::runtime_error("Invalid expression");
        return std::log(value);
    }

    __declspec(dllexport) const char* get_sym() {
        return "ln";
    }
    
}