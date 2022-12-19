#pragma once
#include "FVector.h"
#include "FVector2D.h"
#include "FTransform.h"
#include "TArray.h"
#include <string>

class ATerrain
{

public:
	double Scale = 100.0;
	double UVScale = 1.0;
	double Size = 20.0;
	int Permutation[256];

	ATerrain();
	void Initialize(int p[]);
	void OnConstruction(const FTransform& transform);
	void Destroyed();
	void SpawnTree(std::string treeClass, FVector position);

protected:
	void BeginPlay();

private:
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	//TArray<AGenericTree*> Trees;

	void InitializeTrees();
	void CreateVertices();
	void CreateTriangles();
	//UClass* GetTreeClass(int seed);
	FVector GetActorLocation();

	int TreesLeft;
};

