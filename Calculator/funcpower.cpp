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


class Power : public MathFunction {
public:
    std::string getSymbol() const override {
        return "^";
    }

    OperatorType getOperatorType() const override {
        return OperatorType::Binary;
    }

    int getPrecedence() const override {
        return 4;
    }

    bool isFunction() const override {
        return false;
    }

    double evaluate(const std::vector<double>& args) const override {
        if (args.size() != 2) {
            throw std::runtime_error("Power function requires exactly two arguments.");
        }
        return std::pow(args[0], args[1]);
    }
};