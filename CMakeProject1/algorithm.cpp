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

// Initialize predefined mathematical operators
void Calc::initializeOperators() {
    auto addOperator = [&](const std::string& symbol, OperatorType opType, std::function<double(const std::vector<double>&)> evalFunc) {
        operators.push_back(new Operator(symbol, opType, evalFunc));
        };

    addOperator("+", OperatorType::Binary, [](const std::vector<double>& x) { return x[0] + x[1]; });
    addOperator("-", OperatorType::Binary, [](const std::vector<double>& x) { return x[0] - x[1]; });
    addOperator("*", OperatorType::Binary, [](const std::vector<double>& x) { return x[0] * x[1]; });
    addOperator("/", OperatorType::Binary, [](const std::vector<double>& x) { return x[0] / x[1]; });
}

Calc::Calc() {
    initializeOperators();
}

// Check if a character is an alphabet letter
bool is_alphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Calc::is_operator(const std::string& token) const {
    Operator* op = get_operator(token);
    return (op && op->get_op_type() == OperatorType::Binary);
}

bool Calc::is_function(const std::string& token) const {
    Operator* op = get_operator(token);
    if (op && op->get_op_type() == OperatorType::Unary) {
        return true;
    }
    for (const auto& loadedFunction : loadedFunctions) {
        if (loadedFunction == token) {
            return true;
        }
    }
    return false;
}


// Parse a mathematical expression and calculate the result
double Calc::parse_expression(const std::string& expression) const {
    std::stack<std::string> operators;
    std::vector<std::string> tokens;

    // Loop through each character in the input expression
    for (int i = 0; i < expression.length(); ++i) {
        char sym = expression[i];
        std::string cur_token = { sym };

        if (sym == ' ') {
            continue;
        }

        if (sym == '(') {
            operators.push(cur_token);
        }
        else if (sym == ')') {
            int num_open_parentless = 0;

            while (!operators.empty()) {
                std::string sc = operators.top();
                operators.pop();

                if (sc == "(") {
                    num_open_parentless++;
                    break;
                }
                else {
                    tokens.push_back(sc);
                }
            }

            if (num_open_parentless == 0) {
                throw std::runtime_error("Invalid expression");
            }

            if (!operators.empty()) {
                std::string sc = operators.top();

                if (is_function(sc)) {
                    tokens.push_back(sc);
                    operators.pop();
                }
            }
        }
        // Handle numeric values
        else if (isdigit(sym)) {
            int num_decimal_point = 0;

            while (i < expression.length()) {
                char ch = expression[i + 1];

                if (ch == '.') {
                    num_decimal_point += 1;

                    if (num_decimal_point == 2) {
                        throw std::runtime_error("Invalid expression");
                    }

                    cur_token += ch;
                }
                else if (isdigit(ch)) {
                    cur_token += ch;
                }
                else {
                    break;
                }

                i += 1;
            }

            tokens.push_back(cur_token);
        }
        else if (isalpha(sym)) {
            while (i < expression.length()) {
                char ch = expression[i + 1];

                if (isalpha(ch)) {
                    cur_token += ch;
                }
                else {
                    break;
                }

                i += 1;
            }

            if (is_function(cur_token)) {
                operators.push(cur_token);
            }
            else {
                throw std::runtime_error("Invalid expression");
            }
        }
        // Handle operators
        else if (is_operator(cur_token)) {
            while (!operators.empty()) {
                std::string sc = operators.top();
                std::string sc_str = { sc };

                if (is_operator(sc)) {
                    tokens.push_back(sc_str);
                    operators.pop();
                }
                else {
                    break;
                }
            }

            operators.push(cur_token);
        }
        else {
            throw std::runtime_error("Invalid expression");
        }
    }

    // Process remaining operators
    std::vector<std::string> sub_stack;
    std::vector<double> res_stack;

    while (!operators.empty()) {
        std::string sc = operators.top();
        operators.pop();

        if (sc == "(" || sc == ")") {
            throw std::runtime_error("Invalid expression");
        }

        tokens.push_back(sc);
    }

    for (const auto& token : tokens) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // If the token is a number (including negative numbers), add it to the results stack
            double val = std::stod(token);
            res_stack.push_back(val);
        }
        else if (is_operator(token)) {
            // If the token is an operator
            if (res_stack.size() < 2) {
                throw std::runtime_error("Invalid expression");
            }

            double r = res_stack.back();
            res_stack.pop_back();
            double l = res_stack.back();
            res_stack.pop_back();

            Operator* op = get_operator(token);
            double value = op->parse_exp({ l, r });
            res_stack.push_back(value);
        }
        else if (is_function(token)) {
            // If the token is a function
            if (res_stack.empty()) {
                throw std::runtime_error("Invalid expression");
            }

            double x = res_stack.back();
            res_stack.pop_back();

            Operator* op = get_operator(token);
            double val = op->parse_exp({ x });
            res_stack.push_back(val);
        }
        else {
            throw std::runtime_error("Invalid token: " + token);
        }
    }

    if (res_stack.size() == 1) {
        return res_stack.back();
    }
    else {
        throw std::runtime_error("Invalid expression");
    }
}

// Load a plugin DLL
void Calc::load_plugin(const std::string& filename) {
    std::string plugin_path = path + filename;
    HMODULE load = LoadLibraryA(plugin_path.c_str());

    if (!load) {
        throw std::runtime_error("Could not load plugin");
    }

    const char* (*get_sym_ptr)() = reinterpret_cast<const char* (*)()>(GetProcAddress(load, "get_sym"));
    std::string sym = get_sym_ptr();

    OperatorType* un_or_bin_ptr = reinterpret_cast<OperatorType*>(GetProcAddress(load, "un_or_bin"));
    double (*eptr)(const std::vector<double>&) = reinterpret_cast<double(*)(const std::vector<double>&)>(GetProcAddress(load, "eval"));

    if (!un_or_bin_ptr || !eptr) {
        throw std::runtime_error("Error loading function pointers from plugin");
    }

    OperatorType un_or_bin = *un_or_bin_ptr;

    std::function<double(const std::vector<double>&)> eval = [eptr](const std::vector<double>& x) {
        return eptr(x);
        };

    Operator* op = new Operator(sym, un_or_bin, eval);

    operators.push_back(op);
    loaders.push_back(load);
    loadedFunctions.push_back(sym);

}

// Get an operator by symbol
Operator* Calc::get_operator(const std::string& symbol) const {
    for (auto& op : operators) {
        if (op->get_symbol() == symbol) {
            return op;
        }
    }

    return nullptr;
}

// Get the precedence of an operator
int Calc::get_precedence(const std::string& sym) const {
    Operator* op = get_operator(sym);
    if (op && op->get_op_type() == OperatorType::Binary) {
        return 10; // All binary operators have the same precedence
    }
    else {
        return 10;
    }
}

// Get the type of an operator (unary or binary)
OperatorType Calc::get_op_type(const std::string& sym) const {
    Operator* op = get_operator(sym);
    if (op) {
        return op->get_op_type();
    }
    else {
        return OperatorType::Binary;  // Return a default value (you can change this as needed)
    }
}

Calc::~Calc() {
    // Free loaded plugin libraries
    for (auto const load : loaders) {
        if (load) {
            FreeLibrary(load);
        }
    }
}
