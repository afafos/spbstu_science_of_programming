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

    __declspec(dllexport)std::string sym = "^";
    __declspec(dllexport)OperatorType un_or_bin = OperatorType::Binary;
    __declspec(dllexport)int precedence = 4;
    __declspec(dllexport)bool is_function = false;

    __declspec(dllexport)double eval(const std::vector<double> args) {
        return std::pow(args[0], args[1]);
    }

    __declspec(dllexport) const char* get_sym() {
        return "^";
    }
}