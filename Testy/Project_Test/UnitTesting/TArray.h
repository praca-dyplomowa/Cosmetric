#pragma once
#include <list>
template <class T>
class TArray
{
public:
	std::list<T> objects;
	void Add(T object);
};

