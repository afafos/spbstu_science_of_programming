
#include "pch.h"
#include <cmath>

extern "C" __declspec(dllexport) 
	double calculateSin(double angle) {
		return sin(angle);
	}
