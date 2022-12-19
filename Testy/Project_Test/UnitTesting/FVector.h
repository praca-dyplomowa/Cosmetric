#pragma once
class FVector
{
public:
	float X, Y, Z;
	FVector(float x, float y, float z) : X(x), Y(y), Z(z) {};

	bool operator==(const FVector& other);
};

