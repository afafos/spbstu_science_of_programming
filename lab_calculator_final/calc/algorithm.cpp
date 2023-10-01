#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#include <cctype>
#include <windows.h>
#include "struct.h"

MathFunction calculateSin = nullptr;
MathFunction calculateCos = nullptr;
MathFunction calculateTan = nullptr;
MathFunction calculateLn = nullptr;
MathFunction1 calculatePower = nullptr;

double Calculator::evaluate(const std::string& expression) {
    this->expression = expression;
    index = 0;
    return parseExpression();
}

double Calculator::parseExpression() {
    std::stack<double> values;
    std::stack<char> operators;

    bool expectingOperand = true; // Flag indicating that operand is expected

    while (index < expression.length()) {
        char currentChar = expression[index];

        if (isspace(currentChar)) {
            index++;
        }
        else if (isdigit(currentChar) || currentChar == '.') {
            if (!expectingOperand) {
                throw std::runtime_error("Invalid expression");
            }
            values.push(parseNumber());
            expectingOperand = false; // Waiting for operator
        }
        else if (currentChar == '(') {
            operators.push(currentChar);
            index++;
            expectingOperand = true;
        }
        else if (currentChar == ')') {
            while (!operators.empty() && operators.top() != '(') {
                applyOperator(values, operators);
            }
            if (!operators.empty()) {
                operators.pop();
            }
            index++;
            expectingOperand = false;
        }
        else if (currentChar == '+' || currentChar == '-') {
            if (expectingOperand) {
                // This is a unary operator 
                if (currentChar == '-') {
                    operators.push('~'); // Use '~' to denote unary minus
                }
                index++;
            }
            else {
                while (!operators.empty() && precedence(operators.top()) >= precedence(currentChar)) {
                    applyOperator(values, operators);
                }
                operators.push(currentChar);
                index++;
                expectingOperand = true;
            }
        }
        else if (currentChar == '*' || currentChar == '/') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(currentChar)) {
                applyOperator(values, operators);
            }
            operators.push(currentChar);
            index++;
            expectingOperand = true;
        }
        else if (currentChar == 's' && expression.substr(index, 3) == "sin") {
            operators.push(currentChar);
            index += 3;
        }
        else if (currentChar == 'c' && expression.substr(index, 3) == "cos") {
            operators.push(currentChar);
            index += 3;
        }
        else if (currentChar == 't' && expression.substr(index, 3) == "tan") {
            operators.push(currentChar);
            index += 3;
        }
        else if (currentChar == 'l' && expression.substr(index, 2) == "ln") {
            operators.push(currentChar);
            index += 2;
        }
        else if (currentChar == '^') {
            while (!operators.empty() && precedence(operators.top()) >= precedence(currentChar)) {
                applyOperator(values, operators);
            }
            operators.push(currentChar);
            index++;
            expectingOperand = true;
        }

        else {
            throw std::runtime_error("Invalid character detected");
        }
    }

    while (!operators.empty()) {
        applyOperator(values, operators);
    }

    if (values.size() != 1 || !operators.empty()) {
        throw std::runtime_error("Invalid expression");
    }

    return values.top();
}

double Calculator::parseNumber() {
    size_t endIndex = index;
    while (endIndex < expression.length() && (isdigit(expression[endIndex]) || expression[endIndex] == '.')) {
        endIndex++;
    }

    double number = std::stod(expression.substr(index, endIndex - index));
    index = endIndex;
    return number;
}

int Calculator::precedence(char op) {
    if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    else if (op == 's' || op == '^' || op == 'c' || op == 'l' || op == 't') {
        return 3;
    }

    return 0;
}

void Calculator::applyOperator(std::stack<double>& values, std::stack<char>& operators) {
    char op = operators.top();
    operators.pop();

    if (op == 's') {
        if (values.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        double operand = values.top();
        values.pop();
        values.push(calculateSin(operand));
    }
    else if (op == 'c') {
        if (values.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        double operand = values.top();
        values.pop();
        values.push(calculateCos(operand));
    }
    else if (op == 't') {
        if (values.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        double operand = values.top();
        values.pop();
        values.push(calculateTan(operand));
    }
    else if (op == 'l') {
        if (values.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        double operand = values.top();
        values.pop();
        values.push(calculateLn(operand));
    }
    else if (op == '^') {
        if (values.size() < 2) {
            throw std::runtime_error("Invalid expression");
        }
        double exponent = values.top();
        values.pop();
        double base = values.top();
        values.pop();
        values.push(calculatePower(base, exponent));
    }
    else if (op == '~') { // Process unary minus
        if (values.empty()) {
            throw std::runtime_error("Invalid expression");
        }
        double operand = values.top();
        values.pop();
        values.push(-operand);
    }
    else {
        if (values.size() < 2) {
            throw std::runtime_error("Invalid expression");
        }
        double operand2 = values.top();
        values.pop();
        double operand1 = values.top();
        values.pop();

        switch (op) {
        case '+':
            values.push(operand1 + operand2);
            break;
        case '-':
            values.push(operand1 - operand2);
            break;
        case '*':
            values.push(operand1 * operand2);
            break;
        case '/':
            if (operand2 == 0) {
                throw std::runtime_error("Division by zero");
            }
            values.push(operand1 / operand2);
            break;
        }
    }
}

