#include "worldgen/Fractal.h"

#include "math/Random.h"
#include "math/Constants.h"

#include <memory>

int QuickOverflow(int value, int max)
{
	if (value >= max) return 0;
	if (value < 0) return max;
	else return value;
}
Fractal::Fractal(int size, float factor, float factorDecay) : size(size)
{
	int subdivision = size / 2;
	values = (float*)malloc(size * size * sizeof(float));
	values[0] = Random::RandomFloat(0.0f, 1.0f);
	while (subdivision > 0)
	{
		for (int i = 0; i < (size / subdivision) + 1; i++)
		{
			for (int j = 0; j < (size / subdivision) + 1; j++)
			{
				if (i & 1 && j & 1)
				{
					int x = QuickOverflow(i * subdivision, size);
					int y = QuickOverflow(j * subdivision, size);

					int left = QuickOverflow((i - 1) * subdivision, size);
					int right = QuickOverflow((i + 1) * subdivision, size);
					int top = QuickOverflow((j - 1) * subdivision, size);
					int bottom = QuickOverflow((j + 1) * subdivision, size);

					values[size * x + y] = ((values[size * left + top] +
						values[size * right + top] +
						values[size * left + bottom] +
						values[size * right + bottom]) * (float)factor / 4.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				} else if (i & 1)
				{
					int x = QuickOverflow(i * subdivision, size);
					int y = QuickOverflow(j * subdivision, size);

					int left = QuickOverflow((i - 1) * subdivision, size);
					int right = QuickOverflow((i + 1) * subdivision, size);

					values[size * x + y] = ((values[size * left + y] +
						values[size * right + y]) * (float)factor / 2.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				} else if (j & 1)
				{
					int x = QuickOverflow(i * subdivision, size);
					int y = QuickOverflow(j * subdivision, size);

					int top = QuickOverflow((j - 1) * subdivision, size);
					int bottom = QuickOverflow((j + 1) * subdivision, size);

					values[size * x + y] = ((values[size * x + top] +
						values[size * x + bottom]) * (float)factor / 2.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				}
			}
		}
		subdivision /= 2;
		factor *= factorDecay;
	}

	average = Average();
	stdev = Stdev(average);
}

Fractal::~Fractal()
{
	if (values) free(values);
}

float Fractal::Average()
{
	float avg = 0.0f;

	for (int i = 0; i < size - 1; i++)
	{
		float rowAvg = 0.0f;
		for (int j = 0; j < size - 1; j++)
		{
			rowAvg += values[size * i + j];
		}
		avg += (rowAvg / (size - 1));
	}

	return avg / (size - 1);
}

float Fractal::Stdev()
{
	return Stdev(Average());
}

float Fractal::Stdev(float avg)
{
	float stdev = 0.0f;

	for (int i = 0; i < size - 1; i++)
	{
		for (int j = 0; j < size - 1; j++)
		{
			stdev += powf((values[size * i + j] - avg), 2);
		}
	}

	stdev /= (size * size);
	stdev = sqrtf(stdev);

	return stdev;
}

float Fractal::Value(float latitude, float longitude)
{
	return values[size * (int)(latitude * (float)size) + (int)(longitude * (float)size)];
}

float NormalToLatitude(glm::vec3 normal)
{
	return (atan2f(normal.y, sqrtf(powf(normal.x, 2) + powf(normal.z, 2))) + Constants::PI / 2.0f) / (1.0f * Constants::PI);
}

float NormalToLongitude(glm::vec3 normal)
{
	return (atan2f(normal.z, normal.x) + Constants::PI) / (2.0f * Constants::PI);
}