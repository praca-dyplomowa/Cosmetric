// Fill out your copyright notice in the Description page of Project Settings.


#include "TreeManager.h"
#include "Singleton.h"
#include "PerlinNoise.h"
#include "SpruceLikeTree.h"

#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UTreeManager::UTreeManager() {
}


//put all the tree classes here
UClass* UTreeManager::treeClasses[] = {
	ASpruceLikeTree::StaticClass()
};

UClass* UTreeManager::getClass(int seed)
{
	auto stream = FRandomStream(
		seed
	);
	int i = stream.RandRange(0, 0);
	switch (i)
	{
	case 0:
		return ASpruceLikeTree::StaticClass();
	default:
		break;
	}
	return nullptr;
}


void UTreeManager::Initialize(int(&permutation)[256], FVector terrainPosition, FVector2D terrainSize)
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
	auto stream = FRandomStream(
		seed
		+ terrainPosition.X
		+ terrainPosition.Y
		+ terrainPosition.Z
	);
	auto treeNum = stream.RandRange(0, 3);
	for (int i = 0; i < treeNum; i++) {
		FVector position = stream.GetUnitVector();
		position.X *= terrainSize.X;
		position.Y *= terrainSize.Y;
		position += terrainPosition;
		position.Z = PerlinNoise::Noise(position.X / terrainSize.X, position.Y / terrainSize.Y, permutation) * 500;
		UClass* boop = getClass(seed
			+ terrainPosition.X
			+ terrainPosition.Y
			+ terrainPosition.Z);
		AGenericTree* tree = GetWorld()->SpawnActor<AGenericTree>(boop, FTransform(position));
		if (tree) {
			tree->Materials[0] = stream.GetFraction() * 20;
			tree->Materials[1] = stream.GetFraction() * 30;
			tree->Name = FString("Drzewo");
			Trees.Add(tree);
		}
	}
}

void UTreeManager::DestroyTrees()
{
	if (!Trees.IsEmpty())
	{
		for (AGenericTree* tree : Trees)
			tree->Destroy();
		Trees.Empty();
	}
}

