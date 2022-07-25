#include "pch.h"
#include <math.h>


#define ROWS 200
#define COLS 200
#define DX 1.0f
#define DZ 1.0f
#define START_X (-ROWS / 2)
#define START_Z (-COLS / 2)


extern "C" __declspec(dllexport) void gridmesh_sizeUIpUIp(unsigned int* rows, unsigned int* cols)
{
	*rows = ROWS;
	*cols = COLS;
}

extern "C" __declspec(dllexport) void potential_sizeUIpUIp(
	unsigned int* rows,
	unsigned int* cols
)
{
	*rows = ROWS;
	*cols = COLS;
}

extern "C" __declspec(dllexport) void gridmesh_dataFp(float* p)
{
	for (size_t r = 0; r < ROWS; ++r)
	{
		for (size_t c = 0; c < COLS; ++c)
		{
			const float x = (float)c * DX + START_X;
			const float z = (float)r * DZ + START_Z;
			const float cof = x * x + z * z;

			*p++ = x;
			*p++ = 5000 / cof * sin(double((cof) / 100.0));
			*p++ = z;
		}
	}
}

extern "C" __declspec(dllexport) void potential_dataFp(float* p)
{
	static const float cof = 100.0f;

	for (size_t iz = 0; iz < ROWS; ++iz)
		for (size_t ix = 0; ix < COLS; ++ix)
		{
			const float x = ix * DX + START_X;
			const float z = iz * DZ + START_Z;
			const float r2 = sqrt(x * x + z * z);

			*p++ = x;
			*p++ = -cof / r2;
			*p++ = z;
		}
}