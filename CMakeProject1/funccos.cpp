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

    __declspec(dllexport)std::string sym = "cos";
    __declspec(dllexport) OperatorType un_or_bin = OperatorType::Unary;
    __declspec(dllexport)bool is_function = true;
    __declspec(dllexport)double eval(const std::vector<double> x) {
        return std::cos(x[0]);
    }

    __declspec(dllexport) const char* get_sym() {
        return "cos";
    }
}