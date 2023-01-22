#include "pch.h"
#include "CppUnitTest.h"

#include <cmath>
#include <random>
#include <algorithm>

#include "PerlinNoise.h"
#include "Terrain.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTesting
{
	TEST_CLASS(PerlinNoiseTest)
	{
	public:
		
		TEST_METHOD(Lerp_A_B_0_is_A)
		{
			double a = 1.0;
			double b = 0.0;
			double t = 0.0;
			double r = PerlinNoise::Lerp(a, b, t);
			Assert::AreEqual(a, r);
		}
		TEST_METHOD(Lerp_A_B_1_is_B)
		{
			double a = 1.0;
			double b = 2.0;
			double t = 1.0;
			double r = PerlinNoise::Lerp(a, b, t);
			Assert::AreEqual(b, r);
		}
		TEST_METHOD(Lerp_1_2_0_5_is_1_5)
		{
			double a = 1.0;
			double b = 2.0;
			double t = 0.5;
			double r = PerlinNoise::Lerp(a, b, t);
			Assert::AreEqual(1.5, r);
		}
		TEST_METHOD(Fade_0_is_0)
		{
			double a = 0.0;
			double r = PerlinNoise::Fade(a);
			Assert::AreEqual(a, r);
		}
		TEST_METHOD(Fade_1_is_1)
		{
			double a = 1.0;
			double r = PerlinNoise::Fade(a);
			Assert::AreEqual(a, r);
		}
		TEST_METHOD(Fade_fraction_is_smaller_than_1)
		{
			bool f = true;
			for (float i = 1.0; i <= 99.0; i += 1.0)
			{
				double a = i/100.0;
				double r = PerlinNoise::Fade(a);
				if (f && r >= 1.0) f = false;
			}
			Assert::AreEqual(f, true);
		}
		TEST_METHOD(Fade_fraction_is_greater_than_0)
		{
			bool f = true;
			for (float i = 1.0; i <= 99.0; i += 1.0)
			{
				double a = i / 100.0;
				double r = PerlinNoise::Fade(a);
				if (f && r <= 0.0) f = false;
			}
			Assert::AreEqual(f, true);
		}
		TEST_METHOD(ConstantVector_return_a_2D_vector)
		{
			FVector2D expected(1.0, 1.0);
			FVector2D tested = PerlinNoise::ConstVector(0);
			bool f = tested == expected;
			Assert::AreEqual(f, true);
		}
		TEST_METHOD(ConstantVector_works_for_over_8)
		{
			FVector2D expected(-1.0, 1.0);
			FVector2D tested = PerlinNoise::ConstVector(9);
			bool f = tested == expected;
			Assert::AreEqual(true,f);
		}
		TEST_METHOD(NoiseIsConstantInSamePlace)
		{
			int x = 0.5;
			int y = 0.5;
			int Permutation[256];
			int Seed = 0;
			for (int i = 0; i < 256; i++)
			{
				Permutation[i] = i;
			}
			std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));

			Assert::AreEqual(PerlinNoise::Noise(x, y, Permutation), PerlinNoise::Noise(x, y, Permutation));
		}
		TEST_METHOD(NoiseIsEqualTo0ForIntegerInput)
		{
			int x = 1.0;
			int y = 1.0;
			int Permutation[256];
			int Seed = 0;
			for (int i = 0; i < 256; i++)
			{
				Permutation[i] = i;
			}
			std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));

			Assert::AreEqual(0.0, PerlinNoise::Noise(x, y, Permutation));
		}
		TEST_METHOD(NoiseIsEqualTo0ForIntegerInput_2)
		{
			int x = 3.0;
			int y = 25.0;
			int Permutation[256];
			int Seed = 0;
			for (int i = 0; i < 256; i++)
			{
				Permutation[i] = i;
			}
			std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));

			Assert::AreEqual(0.0, PerlinNoise::Noise(x, y, Permutation));
		}
		TEST_METHOD(Noise_fraction_is_smaller_than_1)
		{
			bool f = true;
			int Permutation[256];
			int Seed = 0;
			for (int i = 0; i < 256; i++)
			{
				Permutation[i] = i;
			}
			std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));
			for (float i = 1.0; i <= 99.0; i += 1.0)
			{
				double x = i / 100.0;
				for (float j = 1.0; j <= 99.0; j += 1.0)
				{
					double y = j / 100.0;
					double r = PerlinNoise::Noise(x,y,Permutation);
					if (f && r >= 1.0) f = false;
				}
			}
			Assert::AreEqual(true, f);
		}
		TEST_METHOD(Noise_fraction_is_greater_equal_to_minus1)
		{
			bool f = true;
			int Permutation[256];
			int Seed = 0;
			for (int i = 0; i < 256; i++)
			{
				Permutation[i] = i;
			}
			std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));
			for (float i = 1.0; i <= 99.0; i += 1.0)
			{
				double x = i / 100.0;
				for (float j = 1.0; j <= 99.0; j += 1.0)
				{
					double y = j / 100.0;
					double r = PerlinNoise::Noise(x, y, Permutation);
					if (f && r <= -1.0) f = false;
				}
			}
			Assert::AreEqual(true, f);
		}
	};
}
