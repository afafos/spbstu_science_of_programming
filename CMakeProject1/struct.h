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

// Enum to represent the type of operator (unary or binary)
enum class OperatorType {
    Unary,
    Binary
};

// Define a class of mathematical operators
class Operator {
public:
    Operator(std::string sym, OperatorType opType, std::function<double(const std::vector<double>&)> eval) :
        sym(sym), opType(opType), evaluate(eval) { }

    std::string get_symbol() const { return sym; }
    OperatorType get_op_type() const { return opType; }
    double parse_exp(const std::vector<double>& expression) const { return evaluate(expression); }
private:
    std::function<double(const std::vector<double>&)> evaluate;
    std::string sym;
    OperatorType opType; // Operator type (Unary or Binary)
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
    OperatorType get_op_type(const std::string& sym) const;
    bool is_operator(const std::string& token) const;
    bool is_function(const std::string& token) const;

public:
    Calc();
    double parse_expression(const std::string& expression) const;
    void load_plugin(const std::string& filename);
    ~Calc();

};

