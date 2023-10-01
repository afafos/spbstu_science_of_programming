
#include "pch.h"
#include <cmath>

extern "C" __declspec(dllexport)
double calculateTan(double angle) {
	return tan(angle);
}
