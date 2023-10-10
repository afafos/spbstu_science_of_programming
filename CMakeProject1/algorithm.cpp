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
    auto addOperator = [&](const std::string& symbol, int un_or_bin, std::function<double(const std::vector<double>&)> evalFunc) {
        operators.push_back(new Operator(symbol, un_or_bin, evalFunc));
        };

    addOperator("+", 2, [](const std::vector<double>& x) { return x[0] + x[1]; });
    addOperator("-", 2, [](const std::vector<double>& x) { return x[0] - x[1]; });
    addOperator("*", 2, [](const std::vector<double>& x) { return x[0] * x[1]; });
    addOperator("/", 2, [](const std::vector<double>& x) { return x[0] / x[1]; });
}

Calc::Calc() {
    initializeOperators();
}

// Check if a character is an alphabet letter
bool is_alphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Parse a mathematical expression and calculate the result
double Calc::parse_expression(const std::string& expression) const {
    std::stack<std::string> operators;
    std::vector<std::string> tokens;
    std::vector<std::string> loadedFunctions;

    // Loop through each character in the input expression
    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        std::string cur = { c };

        if (c == ' ') {
            continue;
        }

        if (c == '(') {
            operators.push(cur);
        }
        else if (c == ')') {
            int k = 0;

            while (!operators.empty()) {
                std::string sc = operators.top();
                operators.pop();

                if (sc == "(") {
                    k++;
                    break;
                }
                else {
                    tokens.push_back(sc);
                }
            }

            if (k == 0) {
                throw std::runtime_error("Invalid expression");
            }

            if (!operators.empty()) {
                std::string sc = operators.top();

                Operator* op = get_operator(sc);
                if (op && op->get_un_or_bin() == 1) {
                    tokens.push_back(sc);
                    operators.pop();
                }
            }
        }
        // Handle numeric values
        else if (isdigit(c)) {
            int c = 0;

            while (i < expression.length()) {
                char ch = expression[i + 1];

                if (ch == '.') {
                    c += 1;

                    if (c == 2) {
                        throw std::runtime_error("Invalid expression");
                    }

                    cur += ch;
                }
                else if (isdigit(ch)) {
                    cur += ch;
                }
                else {
                    break;
                }

                i += 1;
            }

            tokens.push_back(cur);
        }
        else if (is_alphabet(c)) {
            while (i < expression.length()) {
                char ch = expression[i + 1];

                if (is_alphabet(ch)) {
                    cur += ch;
                }
                else {
                    break;
                }

                i += 1;
            }

            Operator* op = get_operator(cur);
            if (op && op->get_un_or_bin() == 1) {
                operators.push(cur);
            }
            else {
                bool isLoadedFunction = false;
                for (const auto& loadedFunction : loadedFunctions) {
                    if (loadedFunction == cur) {
                        isLoadedFunction = true;
                        break;
                    }
                }

                if (!isLoadedFunction) {
                    throw std::runtime_error("Invalid expression");
                }
            }
        }

        // Handle operators
        else if (get_operator(cur)) {
            while (!operators.empty()) {
                std::string sc = operators.top();
                std::string sc_str = { sc };

                Operator* op = get_operator(sc_str);
                if (op && (op->get_un_or_bin() == 2) && ((get_precedence(cur)
                    && (get_precedence(cur) <= get_precedence(sc_str))))) {
                    tokens.push_back(sc_str);
                    operators.pop();
                }
                else {
                    break;
                }
            }

            operators.push(cur);
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
    }for (const auto& token : tokens) {
        if (isdigit(token[0]) || (token[0] == '-' && isdigit(token[1]))) {
            // If the token is a number (including negative numbers), add it to the results stack
            double val = std::stod(token);
            res_stack.push_back(val);
        }
        else if (get_operator(token)) {
            // If the token is an operator or function
            int un_or_bin = get_un_or_bin(token);
            if (res_stack.size() < un_or_bin) {
                throw std::runtime_error("Invalid expression");
            }

            if (un_or_bin == 1) {
                double x = res_stack.back();
                res_stack.pop_back();

                Operator* op = get_operator(token);
                double val = op->parse_exp({ x });
                res_stack.push_back(val);
            }
            else if (un_or_bin == 2) {
                double r = res_stack.back();
                res_stack.pop_back();
                double l = res_stack.back();
                res_stack.pop_back();

                Operator* op = get_operator(token);
                double value = op->parse_exp({ l, r });
                res_stack.push_back(value);
            }
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

    int* un_or_bin_ptr = reinterpret_cast<int*>(GetProcAddress(load, "un_or_bin"));
    double (*eptr)(const std::vector<double>&) = reinterpret_cast<double(*)(const std::vector<double>&)>(GetProcAddress(load, "eval"));

    if (!un_or_bin_ptr || !eptr) {
        throw std::runtime_error("Error loading function pointers from plugin");
    }

    int un_or_bin = *un_or_bin_ptr;

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
    if (op && op->get_un_or_bin() == 2) {
        return 10; // All binary operators have the same precedence
    }
    else {
        return 10;
    }
}

// Get the type of an operator (unary or binary)
int Calc::get_un_or_bin(const std::string& sym) const {
    Operator* op = get_operator(sym);
    if (op) {
        return op->get_un_or_bin();
    }
    else {
        return 0;
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
