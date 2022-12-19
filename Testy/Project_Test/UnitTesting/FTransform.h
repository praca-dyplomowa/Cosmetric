#pragma once
#include "FVector.h"
class FTransform
{
	FVector Location;
	FVector Rotation;

	bool operator==(const FTransform& other);
};

