#include "pch.h"
#include "TArray.h"

template<class T>
void TArray<T>::Add(T object)
{
	objects.insert(object);
}