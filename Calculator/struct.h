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

std::string get_exe_directory();

// Define the path to the plugins directory
const std::string path = get_exe_directory() + "\\plugins\\";

// Enum to represent the type of operator (unary or binary)
enum class OperatorType {
    Unary,
    Binary
};

// Enum to represent the type of operator (left or right associativity)
enum Associativity {
    Left,
    Right,
};

// Define a class of mathematical operators
class Operator {
public:
    Operator(std::string sym, OperatorType opType, std::function<double(const std::vector<double>&)> eval) :
        sym(sym), opType(opType), evaluate(eval) {};

    std::string get_symbol() const { return sym; }
    OperatorType get_op_type() const { return opType; }
    virtual double parse_exp(const std::vector<double>& expression) const { return evaluate(expression); }
private:
    std::function<double(const std::vector<double>&)> evaluate;
    std::string sym;
    OperatorType opType; // Operator type (Unary or Binary)
};


class Unary_operator : public Operator {
private:
    Associativity associativity;

public:
    Unary_operator(std::string sym, Associativity associativity, std::function<double(const std::vector<double>&)> eval) : Operator(sym, OperatorType::Unary, eval), associativity(associativity) {};

    Associativity get_associativity() const { return associativity; }
};


class Binary_operator : public Operator {
private:
    int precedence;

public:
    Binary_operator(std::string sym, int precedence, std::function<double(const std::vector<double>&)> eval) :
        Operator(sym, OperatorType::Binary, eval), precedence(precedence) {};

    int get_precedence() const { return precedence; }
};


class Function_operator : public Operator {
public:
    Function_operator(std::string sym, OperatorType opType, std::function<double(const std::vector<double>&)>& eval) :
        Operator(sym, opType, eval) {};
};

// Define a calculator class that uses operators
class Calc {
private:
    std::vector<Operator*> operators;
    std::vector<HMODULE> loaders;
    std::vector<std::string> get_tokens(const std::string& expression) const;
    double perform_tokens(const std::vector<std::string>& input) const;
    Operator* get_operator(const std::string& sym) const;
    void initializeOperators();
    int get_precedence(const std::string& sym) const;
    OperatorType get_op_type(const std::string& sym) const;
    bool get_associativity(const std::string& sym) const;
    bool is_operator(const std::string& sym) const;
    bool is_function(const std::string& sym) const;

public:
    Calc();
    double parse_expression(const std::string& expression) const;
    void load_plugin(const std::string& filename);
    ~Calc();
};