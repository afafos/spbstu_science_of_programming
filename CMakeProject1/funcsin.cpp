#include <string>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <windows.h>
#include "pch.h"
#include <iostream>
using std::string;

extern "C" {

    __declspec(dllexport)std::string sym = "sin";
    __declspec(dllexport)int un_or_bin = 1;
    __declspec(dllexport)double eval(const std::vector<double> x) {
        return std::sin(x[0]);
    }

    __declspec(dllexport) const char* get_sym() {
        return "sin";
    }
}