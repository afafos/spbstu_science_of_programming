#pragma once
#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <exception>
#include <stack>
#include <sstream>
#include <Windows.h>
#include <functional>
#include <typeinfo>
#include "struct.h"

// Define the path to the plugins directory
const std::string path = "plugins\\";

// Define a class of mathematical operators
class Operator {
public:
    Operator(std::string sym, int un_or_bin, std::function<double(const std::vector<double>&)> eval) :
        sym(sym), un_or_bin(un_or_bin), evaluate(eval) { }

    std::string get_symbol() const { return sym; }
    int get_un_or_bin() const { return un_or_bin; }
    double parse_exp(const std::vector<double>& expression) const { return evaluate(expression); }
private:
    std::function<double(const std::vector<double>&)> evaluate;
    std::string sym;
    int un_or_bin; // 1 for unary, 2 for binary
};

// Define a calculator class that uses operators
class Calc {
private:
    std::vector<Operator*> operators;
    std::vector<HMODULE> loaders;
    std::vector<std::string> loadedFunctions;
    Operator* get_operator(const std::string& sym) const;
    void initializeOperators();
    int get_precedence(const std::string& sym) const;
    int get_un_or_bin(const std::string& sym) const;

public:
    Calc();
    double parse_expression(const std::string& expression) const;
    void load_plugin(const std::string& filename);
    ~Calc();

};

