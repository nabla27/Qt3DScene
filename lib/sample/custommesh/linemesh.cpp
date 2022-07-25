#include "pch.h"
#include <math.h>

#define SAMPLES 5000
#define RANGE 500
#define DX ((float)RANGE / (float)SAMPLES)


extern "C" __declspec(dllexport) void samplesUIp(unsigned int* samples)
{
	*samples = SAMPLES;
}

extern "C" __declspec(dllexport) void dataFp(float* p)
{
	for (unsigned int ix = 0; ix < SAMPLES; ++ix)
	{
		static float radius = 1.0f;

		const float x = ix * DX;

		*p++ = x;
		*p++ = radius * sin(x);
		*p++ = radius * cos(x);

		radius += 0.01f;
	}
}



