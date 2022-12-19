#pragma once
#include "FVector2D.h"
static class PerlinNoise
{
public:

		static double Noise(double x, double y, int permutation[]);

		static double Lerp(double a, double b, double t);

		static double Fade(double t);

		static FVector2D ConstVector(int h);
};

