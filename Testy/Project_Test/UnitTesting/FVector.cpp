#include "pch.h"
#include "FVector.h"

bool FVector::operator==(const FVector& other)
{
    return (X == other.X) && (Y == other.Y) && (Z = other.Z);
}
