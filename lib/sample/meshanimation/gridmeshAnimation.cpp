#include "pch.h"
#include <math.h>


#define ROWS 200
#define COLS 200
#define START_X (-ROWS / 2)
#define START_Z (-COLS / 2)
#define DX 1.0f
#define DZ 1.0f

extern "C" __declspec(dllexport) void gridmeshAnimation_sizeUIpUIpF(
	unsigned int* rows,
	unsigned int* cols,
	float time)
{
	*rows = ROWS;
	*cols = COLS;
}

extern "C" __declspec(dllexport) void gridmeshAnimation_dataFpF(
	float* p,
	float time
)
{
	for (unsigned int row = 0; row < ROWS; ++row)
	{
		for (unsigned int col = 0; col < COLS; ++col)
		{
			const float z = row * DZ + START_Z;
			const float x = col * DX + START_X;

			*p++ = x;
			*p++ = 5.0f * (sin(x / 5.0f - time) + cos(z / 5.0f - time));
			*p++ = z;
		}
	}
}

extern "C" __declspec(dllexport) void sizeUIpUIpF()
{
	;
}

extern "C" __declspec(dllexport) void dataFpF()
{
	;
}