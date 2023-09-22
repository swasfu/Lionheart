#include "worldgen/Fractal.h"

#include "math/Random.h"

#include <iostream>

void CreateFractal(ValueMap& map, int size, float factor, float factorDecay)
{
	std::vector<float> mapData(size * size);
	int subdivision = size / 2;
	mapData[0] = Random::RandomFloat(0.0f, 1.0f);
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

					mapData[size * x + y] = ((mapData[size * left + top] +
						mapData[size * right + top] +
						mapData[size * left + bottom] +
						mapData[size * right + bottom]) * (float)factor / 4.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				} else if (i & 1)
				{
					int x = QuickOverflow(i * subdivision, size);
					int y = QuickOverflow(j * subdivision, size);

					int left = QuickOverflow((i - 1) * subdivision, size);
					int right = QuickOverflow((i + 1) * subdivision, size);

					mapData[size * x + y] = ((mapData[size * left + y] +
						mapData[size * right + y]) * (float)factor / 2.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				} else if (j & 1)
				{
					int x = QuickOverflow(i * subdivision, size);
					int y = QuickOverflow(j * subdivision, size);

					int top = QuickOverflow((j - 1) * subdivision, size);
					int bottom = QuickOverflow((j + 1) * subdivision, size);

					mapData[size * x + y] = ((mapData[size * x + top] +
						mapData[size * x + bottom]) * (float)factor / 2.0f +
						Random::RandomFloat(0.0f, 1.0f)) / (factor + 1);
				}
			}
		}
		subdivision /= 2;
		factor *= factorDecay;
	}

	map.width = size;
	map.height = size / 2;
	map.Alloc();

	for (int longitude = 0; longitude < map.width; longitude++)
	{
		for (int latitude = 0; latitude < map.height; latitude++)
		{
			map.values[longitude * map.height + latitude] = mapData[longitude * size + latitude];
		}
	}

	map.Average();
	map.Stdev(map.average);
}
