// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain/Terrain.h"
#include "ProceduralMeshComponent.h"
#include <random>
#include <algorithm>
#include <cmath>
#include "Singleton.h"
#include "Math/RandomStream.h"
#include "Tree/SpruceLikeTree.h"
#include "Tree/OakLikeTree.h"
#include "Terrain/TerrainMenager.h"
#include "Kismet/GameplayStatics.h"
#include "PerlinNoise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");

	UMaterial* mt = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Ground_Moss"));
	ProceduralMesh->SetMaterial(0, mt);
	SetRootComponent(ProceduralMesh);
}

void ATerrain::Initialize(int p[])
{
	for (int i = 0; i < 256; i++)
	{
		Permutation[i] = p[i];
	}
	CreateVertices();
	CreateTriangles();
}

void ATerrain::BeginPlay()
{
	Super::BeginPlay();
}

void ATerrain::OnConstruction(const FTransform& transform) {
	Super::OnConstruction(transform);

	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UV0, TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	InitializeTrees();
}

void ATerrain::Destroyed()
{
	for (auto tree : Trees) {
		if (tree->IsValidLowLevelFast()) {
			tree->Destroy();
		}
	}
	Super::Destroyed();
}

void ATerrain::SpawnTree(UClass* treeClass, FVector position)
{
	auto tree = GetWorld()->SpawnActor<AGenericTree>(treeClass, FTransform(position));
	if (tree) {
		tree->SetOwner(this);
		Trees.Add(tree);
	}
}

void ATerrain::InitializeTrees()
{
	int seed = 0;
	TArray<AActor*> ActorsToFind;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASingleton::StaticClass(), ActorsToFind);
	for (AActor* Singleton : ActorsToFind)
	{
		ASingleton* single = Cast<ASingleton>(Singleton);
		if (single)
		{
			seed = single->Seed;
		}
	}
	auto terrainPosition = GetActorLocation();
	auto terrainSize = Scale * Size;
	auto stream = FRandomStream(
		seed
		+ terrainPosition.X
		+ terrainPosition.Y
		+ terrainPosition.Z
	);
	auto treeNum = stream.RandRange(0, 3);
	for (int i = 0; i < treeNum; i++) {
		FVector position = stream.GetUnitVector();
		position.X *= terrainSize;
		position.Y *= terrainSize;
		position += terrainPosition;
		position.Z = PerlinNoise::Noise(position.X / terrainSize, position.Y / terrainSize, Permutation) * 500;
		UClass* treeStaticClass = GetTreeClass(seed +
			position.X +
			position.Y +
			position.Z
		);
		SpawnTree(treeStaticClass, position);
	}
}

void ATerrain::CreateVertices()
{
	for (int X = 0; X <= Size; ++X)
	{
		for (int Y = 0; Y <= Size; ++Y)
		{
			double n = PerlinNoise::Noise((GetActorLocation().X + (double)X * Scale) / (Size * Scale), 
				(GetActorLocation().Y + (double)Y * Scale) / (Size * Scale), Permutation) * 500;
			Vertices.Add(FVector(X * Scale, Y * Scale, n));
			UV0.Add(FVector2D(X * UVScale, Y * UVScale));
		}
	}
}

void ATerrain::CreateTriangles()
{
	int Vertex = 0;
	for (int X = 0; X < Size; ++X)
	{
		for (int Y = 0; Y < Size; ++Y)
		{
			Triangles.Add(Vertex);//Bottom left corner
			Triangles.Add(Vertex + 1);//Bottom right corner
			Triangles.Add(Vertex + Size + 1);//Top left corner

			Triangles.Add(Vertex + 1);//Bottom right corner
			Triangles.Add(Vertex + Size + 2);//Top right corner
			Triangles.Add(Vertex + Size + 1);//Top left corner
			++Vertex;
		}
		++Vertex;
	}
}

UClass* ATerrain::GetTreeClass(int seed)
{
	auto stream = FRandomStream(
		seed
	);

	int i = stream.RandRange(0, 1);
	switch (i)
	{
	case 0:
		return ASpruceLikeTree::StaticClass();
	case 1:
		return AOakLikeTree::StaticClass();
	default:
		break;
	}
	return nullptr;
}
