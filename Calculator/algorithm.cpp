#include "struct.h"

std::string get_exe_directory() {
    WCHAR exePath[MAX_PATH];
    GetModuleFileNameW(NULL, exePath, MAX_PATH);

    // Extract the directory from the path to the exe file
    std::wstring exeDir = exePath;
    size_t pos = exeDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        exeDir = exeDir.substr(0, pos);
    }
    std::string narrowExeDir(exeDir.begin(), exeDir.end());
    return narrowExeDir;
}

void Calc::initializeOperators() {
    operators.emplace_back(new Binary_operator("+", 2, [](const std::vector<double>& x) { return x[0] + x[1]; }));
    operators.emplace_back(new Binary_operator("-", 2, [](const std::vector<double>& x) { return x[0] - x[1]; }));
    operators.emplace_back(new Binary_operator("*", 3, [](const std::vector<double>& x) { return x[0] * x[1]; }));
    operators.emplace_back(new Binary_operator("/", 3, [](const std::vector<double>& x) { return x[0] / x[1]; }));
}

Calc::Calc() {
    initializeOperators();
}

// Check if a character is an alphabet letter
bool is_alphabet(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

bool Calc::is_operator(const std::string& sym) const {
    if (sym == "(" || sym == ")")
        return false;
    Operator* op = get_operator(sym);
    return dynamic_cast<Unary_operator*>(op) != nullptr || dynamic_cast<Binary_operator*>(op) != nullptr;
}

bool Calc::is_function(const std::string& symbol) const {
    Operator* op = get_operator(symbol);
    return dynamic_cast<Function_operator*>(op) != nullptr;
}

// Parse a mathematical expression and calculate the result
double Calc::parse_expression(const std::string& expression) const {
    std::vector<std::string> tokens = get_tokens(expression);
    double result = perform_tokens(tokens);
    return result;
}

// Split a mathematical expression into tokens for parsing.
std::vector<std::string> Calc::get_tokens(const std::string& expression) const {
    std::stack<std::string> operators;
    std::vector<std::string> tokens;

    // Loop through each character in the input expression
    for (int i = 0; i < expression.length(); ++i) {
        char sym = expression[i];
        std::string cur_token = { sym };

        // Ignore spaces
        if (sym == ' ') {
            continue;
        }

        // Handle opening parentheses
        if (sym == '(') {
            operators.push(cur_token);
        }
        // Handle closing parentheses
        else if (sym == ')') {
            int num_open_parentless = 0;

            // Pop operators from the stack until an opening parenthesis is encountered
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

            // If no opening parenthesis was found, the expression is invalid
            if (num_open_parentless == 0) {
                throw std::runtime_error("Invalid expression");
            }

            // If the operator stack is not empty, check for functions
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

            // Read the complete numeric token, including decimal points
            while (i < expression.length()) {
                char ch = expression[i + 1];

                if (ch == '.') {
                    num_decimal_point += 1;

                    // Ensure there are at most two decimal points in a number
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

            // Add the numeric token to the list of tokens
            tokens.push_back(cur_token);
        }
        // Handle function names
        else if (isalpha(sym)) {
            // Read the complete function name
            while (i < expression.length()) {
                char ch_function = expression[i + 1];

                if (isalpha(ch_function)) {
                    cur_token += ch_function;
                }
                else {
                    break;
                }

                i += 1;
            }

            // If the token is a valid function, add it to the operator stack
            if (is_function(cur_token)) {
                operators.push(cur_token);
            }
        }
        // Handle operators
        else if (is_operator(cur_token)) {
            // Pop operators from the stack and add them to the token list
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

            // Push the current operator onto the operator stack
            operators.push(cur_token);
        }

        else {
            throw std::runtime_error("Invalid expression");
        }
    }

    // Process any remaining operators on the stack
    while (!operators.empty()) {
        std::string sc = operators.top();
        operators.pop();

        // Ensure there are no unbalanced parentheses in the expression
        if (sc == "(" || sc == ")") {
            throw std::runtime_error("Invalid expression");
        }

        // Add the remaining operators to the token list
        tokens.push_back(sc);
    }

    return tokens;
}


// Calculating a mathematical expression represented in reverse Polish notation
double Calc::perform_tokens(const std::vector<std::string>& tokens) const {
    std::string input = "";
    for (const auto& token : tokens) {
        input += token + "|";
    }

    std::vector<std::string> sub_stack(input.length());
    std::vector<double> res_stack(input.length());
    int stack_length = 0;
    int result_number = 0;

    for (int i = 0; i < input.length(); ++i) {
        char ch = input[i];
        std::string cur_token = { ch };

        // Check if the token is a numeric value
        if (isdigit(ch)) {
            char ch_cur = input[i + 1];

            // Read the complete numeric token
            while (ch_cur != '|') {
                cur_token += ch_cur;
                i += 1;
                ch_cur = input[i + 1];
            }

            i += 1;

            // Store the numeric token in the sub-stack and result stack
            sub_stack[stack_length] = cur_token;
            res_stack[stack_length] = std::stod(cur_token);
            stack_length += 1;
        }
        else {
            // Check if the token is an operator or a function
            if (isalpha(ch)) {
                char ch_cur = input[i + 1];

                // Read the complete token
                while (ch_cur != '|') {
                    cur_token += ch_cur;
                    i += 1;
                    ch_cur = input[i + 1];
                }

                sub_stack[stack_length] = cur_token;
            }

            i += 1;

            // If the token is an operator or a function, perform the corresponding operation
            if (is_operator(cur_token) || is_function(cur_token)) {
                OperatorType arity = get_op_type(cur_token);
                double val = 0;
                std::string res = "(" + std::to_string(result_number) + ")";
                result_number += 1;

                // Perform the operation based on its arity
                if (arity == OperatorType::Unary) {
                    double arg = res_stack[stack_length - 1];
                    stack_length -= 1;
                    Operator* op = get_operator(cur_token);
                    val = op->parse_exp({ arg });
                }
                else {
                    double left_arg = res_stack[stack_length - 2];
                    double right_arg = res_stack[stack_length - 1];

                    Operator* op = get_operator(cur_token);
                    val = op->parse_exp({ left_arg, right_arg });

                    stack_length -= 2;
                }

                // Store the result in the sub-stack and result stack
                sub_stack[stack_length] = res;
                res_stack[stack_length] = val;
                stack_length += 1;
            }
        }
    }

    if (stack_length == 1) {
        return res_stack[stack_length - 1];
    }

    throw std::runtime_error("Invalid expression");
}


// Load a plugin DLL
void Calc::load_plugin(const std::string& filename) {
    std::string plugin_path = path + filename;
    HMODULE load = LoadLibraryA(plugin_path.c_str());

    if (!load) {
        throw std::runtime_error("Could not load plugin");
    }

    const char* (*get_sym_ptr)() = (const char* (*)())GetProcAddress(load, "get_sym");
    std::string sym = get_sym_ptr();

    OperatorType* un_or_bin_ptr = reinterpret_cast<OperatorType*>(GetProcAddress(load, "un_or_bin"));
    double (*eptr)(const std::vector<double>&) = reinterpret_cast<double(*)(const std::vector<double>&)>(GetProcAddress(load, "eval"));

    if (!un_or_bin_ptr || !eptr) {
        throw std::runtime_error("Error loading function pointers from plugin");
    }

    void* is_function_ptr = GetProcAddress(load, "is_function");
    OperatorType un_or_bin = *reinterpret_cast<OperatorType*>(un_or_bin_ptr);

    std::function<double(const std::vector<double>&)> eval = reinterpret_cast<double (*)(const std::vector<double> &)>(eptr);
    bool is_function = *reinterpret_cast<bool*>(is_function_ptr);

    Operator* op = nullptr;

    if (is_function) {
        op = new Function_operator(sym, un_or_bin, eval);
    }
    else if (un_or_bin == OperatorType::Unary) {
        FARPROC associativity_ptr = GetProcAddress(load, "associativity");
        if (!associativity_ptr) {
            throw std::runtime_error("Error getting associativity");
        }
        Associativity associativity = *reinterpret_cast<Associativity*>(associativity_ptr);

        op = new Unary_operator(sym, associativity, eval);
    }
    else {
        FARPROC precedence_ptr = GetProcAddress(load, "precedence");
        if (!precedence_ptr) {
            throw std::runtime_error("Error getting precedence");
        }
        int precedence = *reinterpret_cast<int*>(precedence_ptr);

        op = new Binary_operator(sym, precedence, eval);
    }

    operators.push_back(op);
    loaders.push_back(load);
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
    if (op->get_op_type() == OperatorType::Binary) {
        return dynamic_cast<Binary_operator*>(op)->get_precedence();
    }
    else {
        return 10;
    }
}

// Get the type of an operator (unary or binary)
OperatorType Calc::get_op_type(const std::string& sym) const {
    Operator* op = get_operator(sym);
    return op->get_op_type();
}

// Get the type of an associativity
bool Calc::get_associativity(const std::string& sym) const {
    Operator* op = get_operator(sym);
    OperatorType arity = get_op_type(sym);
    if (arity == OperatorType::Unary) {
        return dynamic_cast<Unary_operator*>(op)->get_associativity() == Left;
    }
    else {
        return true;
    }
}


Calc::~Calc() {

    // Free loaded plugin libraries
    for (auto const loader : loaders) {
        if (loader) {
            FreeLibrary(loader);
        }
    }

    // Clean up operators
    for (Operator* op : operators) {
        delete op;
    }
}