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

class MathFunction {
public:
    virtual ~MathFunction() = default;

    virtual std::string getSymbol() const = 0;
    virtual OperatorType getOperatorType() const = 0;
    virtual int getPrecedence() const = 0;
    virtual bool isFunction() const = 0;
    virtual double evaluate(const std::vector<double>& args) const = 0;
};

class NaturalLog : public MathFunction {
public:
    std::string getSymbol() const override {
        return "ln";
    }

    OperatorType getOperatorType() const override {
        return OperatorType::Unary;
    }

    int getPrecedence() const override {
        return 0; 
    }

    bool isFunction() const override {
        return true;
    }

    double evaluate(const std::vector<double>& args) const override {
        if (args.size() != 1) {
            throw std::runtime_error("Natural logarithm function requires exactly one argument.");
        }
        double value = args[0];
        if (value <= 0) {
            throw std::runtime_error("Invalid expression");
        }
        return std::log(value);
    }
};