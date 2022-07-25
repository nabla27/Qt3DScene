#include "pch.h"
#include <math.h>

#define X 0
#define Y 1
#define Z 2

extern "C" __declspec(dllexport) void orbitAnimation_Fp(float* p)
{
	static float cof = 5.0f;
	static float radius = 5.0f;
	float* pos = p;
	float* rot = p + 3;
	float* scale = p + 6;
	float time = *(p + 9);

	pos[X] = radius * sin(cof * time);
	pos[Z] = radius * cos(cof * time);
	scale[Y] = radius * fabs(sin(cof * time));
}