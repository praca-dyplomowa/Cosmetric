// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/TreeManager.h"
#include "Singleton.h"
#include "PerlinNoise.h"
#include "Tree/SpruceLikeTree.h"

#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
UTreeManager::UTreeManager() {
}

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
	cleared = false;
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
		UClass* treeStaticClass = getClass(seed +
			position.X +
			position.Y + 
			position.Z
		);
		AGenericTree* tree = GetWorld()->SpawnActor<AGenericTree>(treeStaticClass, FTransform(position));
		if (tree) {
			tree->Name = FString("Drzewo");
			Trees.Add(tree);
		}
	}
}

void UTreeManager::DestroyTrees()
{

	if (!Trees.IsEmpty() && !cleared)
	{
		cleared = true;
		int l = Trees.Num();
		for (int i = 0; i<l; i++)
			if(IsValid(Trees[i]))
				Trees[i]->Destroy();
		Trees.Empty();
	}
}

