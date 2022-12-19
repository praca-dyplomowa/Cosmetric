#include "pch.h"
#include "FVector2D.h"

FVector2D::FVector2D(float a, float b)
{
	this->X = a;
	this->Y = b;
}

float FVector2D::Dot(FVector2D other)
{
	return X * other.X + Y * other.Y;
}

bool FVector2D::operator==(const FVector2D& other)
{
	return (X == other.X) && (Y == other.Y);
}
