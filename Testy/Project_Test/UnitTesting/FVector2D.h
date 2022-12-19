#pragma once
class FVector2D
{
public:
	float X;
	float Y;
	FVector2D(float a, float b);
	float Dot(FVector2D other);
	bool operator==(const FVector2D& other);
};

