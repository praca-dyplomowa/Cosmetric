// Fill out your copyright notice in the Description page of Project Settings.


#include "Terrain/Terrain.h"
#include "ProceduralMeshComponent.h"
#include <random>
#include <algorithm>
#include <cmath>
#include "GUI/MyGameInstance.h"
#include "Math/RandomStream.h"
#include "Tree/SpruceLikeTree.h"
#include "Tree/OakLikeTree.h"
#include "Tree/SpiralConeTree.h"
#include "Tree/RoundTopTree.h"
#include "Terrain/TerrainMenager.h"
#include "PerlinNoise.h"

ATerrain::ATerrain()
{
	PrimaryActorTick.bCanEverTick = false;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMesh");

	UMaterial* mt = LoadObject<UMaterial>(nullptr, TEXT("/Game/Terrain/MTerrainSimple"));
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

void ATerrain::SpawnTree(FVector position, double seed)
{
	UClass* treeClass = GetTreeClass(seed +
		position.X +
		position.Y +
		position.Z
	);
	auto tree = GetWorld()->SpawnActor<AGenericTree>(treeClass, FTransform(position));
	if (tree) {
		tree->SetOwner(this);
		Trees.Add(tree);
	}
}

void ATerrain::InitializeTrees()
{
	int seed = 0;
	TSet<FVector2D> DestroyedTrees;
	auto GameInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());
	if (GameInstance) {
		auto GameInfo = GameInstance->GameInfo;
		if (GameInfo)
		{
			seed = GameInfo->Seed;
			auto thisChunkInfo = GameInfo->ChunkInfo.Find(GetActorLocation());
			if (thisChunkInfo != nullptr) {
				DestroyedTrees = thisChunkInfo->DestroyedTreePositions; // all destroyed trees positions 
			}
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
	TreesLeft = stream.RandRange(0, 3);
	if (DestroyedTrees.Num() == TreesLeft) {
		return;
	}
	for (int i = 0; i < TreesLeft; i++) {
		FVector position = stream.GetUnitVector();
		if (position.X < 0) position.X *= -terrainSize;
		else position.X *= terrainSize;
		if (position.Y < 0) position.Y *= -terrainSize;
		else position.Y *= terrainSize;
		
		position += terrainPosition;
		if (!DestroyedTrees.Contains(FVector2D(position))) {
			position.Z = PerlinNoise::Noise(position.X / terrainSize, position.Y / terrainSize, Permutation) * 500;
			UClass* treeStaticClass = GetTreeClass(seed +
				position.X +
				position.Y +
				position.Z
			);
			SpawnTree(position, seed);
		}
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

	int i = stream.RandRange(0, 3);
	switch (i)
	{
	case 0:
		return ASpruceLikeTree::StaticClass();
	case 1:
		return AOakLikeTree::StaticClass();
	case 2:
		return ASpiralConeTree::StaticClass();
	case 3:
		return ARoundTopTree::StaticClass();
	default:
		break;
	}
	return nullptr;
}
