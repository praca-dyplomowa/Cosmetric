// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree/GenericTree.h"
#include "GameFramework/Actor.h"
#include "Terrain.generated.h"

class UProceduralMeshComponent;
class UMaterialInterface;

UCLASS()
class COSMETRIC_API ATerrain : public AActor
{
	GENERATED_BODY()
	
public:	
	static const inline double Scale = 100.0;
	static const inline double UVScale = 0.2;
	static const inline double Size = 20.0;
	int Permutation[256];

	ATerrain();
	void Initialize(int p[]);
	virtual void OnConstruction(const FTransform& transform) override;
	virtual void Destroyed() override;
	void SpawnTree(FVector position, double seed);

protected:
	virtual void BeginPlay() override;

private:
	UProceduralMeshComponent* ProceduralMesh;
	TArray<FVector> Vertices;
	TArray<int> Triangles;
	TArray<FVector2D> UV0;
	TArray<AGenericTree*> Trees;

	void InitializeTrees();
	void CreateVertices();
	void CreateTriangles();
	UClass* GetTreeClass(int seed);

	int TreesLeft;
};
