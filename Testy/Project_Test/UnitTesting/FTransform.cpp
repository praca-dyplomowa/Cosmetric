#include "pch.h"
#include "FTransform.h"

bool FTransform::operator==(const FTransform& other)
{
    return Location == other.Location && Rotation == other.Rotation;
}
