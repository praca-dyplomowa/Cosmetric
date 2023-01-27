// Fill out your copyright notice in the Description page of Project Settings.

#include "Terrain/TerrainMenager.h"
#include "GUI/MyGameInstance.h"

ATerrainMenager::ATerrainMenager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATerrainMenager::OnConstruction(const FTransform& transform)
{
	Super::OnConstruction(transform);
}

float ATerrainMenager::Noise(float x, float y)
{
	return PerlinNoise::Noise(x, y, Permutation)*500;
}

void ATerrainMenager::Move(FVector2D NewCenter)
{
	FVector2D dCenter = FVector2D(
		NewCenter.X - CenterRegion.X,
		NewCenter.Y - CenterRegion.Y
	);
	ATerrain*** NewRenderedTerrain = new ATerrain ** [Size];;
	for (int y = 0; y < Size; y++)
	{
		NewRenderedTerrain[y] = new ATerrain * [Size];
		for (int x = 0; x < Size; x++)
		{
			if (
				x + dCenter.X >= 0 &&
				x + dCenter.X < Size &&
				y + dCenter.Y >= 0 &&
				y + dCenter.Y < Size
				)
			{
				NewRenderedTerrain[y][x] = RenderedTerrain[y + (int)(dCenter.Y)][x + (int)(dCenter.X)];
			}
			else
			{
				FTransform pos = FTransform(FVector(
					(NewCenter.X + x - RenderDistance) * (ATerrain::Size * ATerrain::Scale),
					(NewCenter.Y + y - RenderDistance) * (ATerrain::Size * ATerrain::Scale),
					0));
				NewRenderedTerrain[y][x] = GetWorld()->SpawnActorDeferred<ATerrain>(
					ATerrain::StaticClass(),
					pos);
				NewRenderedTerrain[y][x]->Initialize(Permutation);
				NewRenderedTerrain[y][x]->FinishSpawning(pos);
			}
		}
	}
	for (int y = 0; y < Size; y++)
	{
		for (int x = 0; x < Size; x++)
		{
			if (
				x - dCenter.X < 0 ||
				x - dCenter.X >= Size ||
				y - dCenter.Y < 0 ||
				y - dCenter.Y >= Size
				)
				if(IsValid(RenderedTerrain[y][x]))
					RenderedTerrain[y][x]->Destroy();
		}
		delete[] RenderedTerrain[y];
	}
	delete[] RenderedTerrain;
	RenderedTerrain = NewRenderedTerrain;
	CenterRegion = NewCenter;
}

// Called when the game starts or when spawned
void ATerrainMenager::BeginPlay()
{
	Super::BeginPlay();
	auto GameInstance = ((UMyGameInstance*)(GetWorld()->GetGameInstance()));
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	if (GameInfo) {
		Seed = GameInfo->Seed;
	}
	if (GameInstance) {
		RenderDistance = GameInstance->RenderDistance;
	}
	for (int i = 0; i < 256; i++)
	{
		Permutation[i] = i;
	}
	std::shuffle(&Permutation[0], &Permutation[255], std::default_random_engine(Seed));

	CenterRegion = FVector2D(0, 0);
	Size = 2 * RenderDistance + 1;
	RenderedTerrain = new ATerrain * *[Size];
	for (int y = 0; y < Size; y++)
	{
		RenderedTerrain[y] = new ATerrain * [Size];
		for (int x = 0; x < Size; x++)
		{
			FTransform pos = FTransform(FVector(
				(x - RenderDistance) * (ATerrain::Size * ATerrain::Scale),
				(y - RenderDistance) * (ATerrain::Size * ATerrain::Scale),
				0)
			);
			RenderedTerrain[y][x] = GetWorld()->SpawnActorDeferred<ATerrain>(
				ATerrain::StaticClass(),
				pos);
			RenderedTerrain[y][x]->Initialize(Permutation);
			RenderedTerrain[y][x]->FinishSpawning(pos);
		}
	}
}

// Called every frame
void ATerrainMenager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector pos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector2D reg = FVector2D(
		floor(pos.X / (ATerrain::Size * ATerrain::Scale)), 
		floor(pos.Y / (ATerrain::Size * ATerrain::Scale))
	);

	if (reg.X != CenterRegion.X || reg.Y != CenterRegion.Y)
	{
		Move(reg);
	}
}
