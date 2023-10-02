
#include "pch.h"
#include <cmath>

extern "C" __declspec(dllexport)
double calculatePower(double base, double exponent) {
	return pow(base, exponent);
}



