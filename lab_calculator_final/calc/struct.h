#pragma once

typedef double (*MathFunction)(double);
typedef double (*MathFunction1)(double, double);
extern MathFunction calculateSin;
extern MathFunction calculateCos;
extern MathFunction calculateTan;
extern MathFunction calculateLn;
extern MathFunction1 calculatePower;


class Calculator {
public:
	double evaluate(const std::string& expression);
private:
	std::string expression;
	size_t index;
	double parseExpression();
	double parseNumber();
	int precedence(char op);
	void applyOperator(std::stack<double>& values, std::stack<char>& operators);
};