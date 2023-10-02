
#include "pch.h"
#include <cmath>

extern "C" __declspec(dllexport)
double calculateCos(double angle) {
	return cos(angle);
}
