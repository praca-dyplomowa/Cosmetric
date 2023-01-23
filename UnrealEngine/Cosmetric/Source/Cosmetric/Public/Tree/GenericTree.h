// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Interfaces/IsCollectable.h"
#include "GenericTree.generated.h"

UENUM(BlueprintType)
enum class ShrinkType : uint8 {
	Linear = 0 UMETA(DisplayName = "Linear"),
	Geometric = 1  UMETA(DisplayName = "Geometric"),
};

USTRUCT(BlueprintType)
struct FTreeComponentInit
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.5"))
	double InitialSegmentSize = 20;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.001", ClampMax = "1"))
	FVector2D VerticalOffsetPercentClamp = { 1, 1 };
	UPROPERTY(EditAnywhere, meta = (ClampMin = "100", ClampMax = "1000"))
	FVector2D HeightBounds = { 150, 350 };
	UPROPERTY(EditAnywhere)
	UStaticMesh* StaticMesh = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterial* Material = nullptr;
};

USTRUCT(BlueprintType)
struct FTreetopInit : public FTreeComponentInit {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTrunkInit : public FTreeComponentInit {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTreeComponentRenderVariables {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "100"))
	double HorizontalStretch = 1;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "180"))
	double RotationDegreesVariance = 0;
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0001", ClampMax = "0.9999"))
	double HorizontalScalingVariance = 0;
};

USTRUCT(BlueprintType)
struct FTreeComponentRender : public FTreeComponentRenderVariables {
	GENERATED_BODY()
public:
	double Height;
	double SegmentSize;
	UInstancedStaticMeshComponent* Instanced;
};

USTRUCT(BlueprintType)
struct FTreetopRenderVariables : public FTreeComponentRenderVariables {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTrunkRenderVariables : public FTreeComponentRenderVariables {
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FTreeParams {
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double Height;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double CalorificValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isPoisonous;

};

UCLASS()
class COSMETRIC_API AGenericTree : public AIsCollectable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties

	AGenericTree();
	void Initialize(
		FTrunkInit trunkInit,
		FTreetopInit treetopInit = FTreetopInit(),
		FTreetopRenderVariables treetopRender = FTreetopRenderVariables(),
		FTrunkRenderVariables trunkRender = FTrunkRenderVariables()
	);
	virtual void Initialize(
		FTreetopInit treetopInit = FTreetopInit(),
		FTrunkInit trunkInit = FTrunkInit()
	);
	virtual void Initialize(
		FTreetopRenderVariables treetopRender = FTreetopRenderVariables(),
		FTrunkRenderVariables trunkRender = FTrunkRenderVariables()
	);
	void Initialize(
		FTrunkRenderVariables trunkRender = FTrunkRenderVariables(),
		FTreetopRenderVariables treetopRender = FTreetopRenderVariables()
	);
	void InitializeSpecies(FString Color, FString Size);
	UFUNCTION(BlueprintCallable)
	FTreeParams GetAverageParams();
protected:
	double GetRandomFromVector(FVector2D& bounds);

	double GetMeshOffset(UStaticMesh* staticMesh, double size);
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Tick(float DeltaTime) override;
	virtual double RenderUp(FTreeComponentRender &generatedStruct, double Offset = 1);
	virtual double RenderTrunk(double offset);
	virtual double RenderTreetop(double offset);
	virtual void OnConstruction(const FTransform& transform) override;
	virtual void OnCollected() override;
	virtual void SaveTreeDestroyed();
	double RenderConeTreetop(double Offset, double InitialTreetopRadius, int PartsOfCircle, ShrinkType type = ShrinkType::Geometric);
	double RenderSpiralTrunk(double Offset, double SpiralRadius, double RotationDegree, ShrinkType type = ShrinkType::Geometric);
	double RenderRoundTreetop(double Offset, int PartsOfCircle);
	virtual void SunsetizeTreetop(double treetopSegmentNum);
	TArray<FVector> PrepareSunsetVectors(FVector beginning, FVector end, int number);
	
	UPROPERTY(EditAnywhere, Category = "Trunk")
	FTreeComponentInit TrunkInit;
	UPROPERTY(EditAnywhere, Category = "Treetop")
	FTreeComponentInit TreetopInit;
	UPROPERTY(EditAnywhere, Category = "Trunk")
	FTreeComponentRender TrunkRender;
	UPROPERTY(EditAnywhere, Category = "Treetop")
	FTreeComponentRender TreetopRender;
	FRandomStream Stream;
	int Seed = 0;
	bool CustomizedColors = false;
	FString Species;
	FString Color;
	FString Size;
	FVector2D WoodAmountClamp;
	FVector2D FoodAmountClamp;
	bool isPoisonous;
private:
	double InitStruct(FTreeComponentInit& init, FTreeComponentRender& render);
	void RandomizeScale();

};

const TMap<FString, FColor> colorMap = {
{TEXT("Andunie"), FColor()}, // sunset treetop, dummy FColor value
{TEXT("Carne"), FColor(252, 15, 3) },
{TEXT("Luine"), FColor(3, 102, 252) },
{TEXT("Laiqua"), FColor(0, 171, 20)},
{TEXT("Laure"), FColor(252, 173, 3)},
{TEXT("Helin"), FColor(54, 0, 166)},
{TEXT("Culuina"), FColor(157, 28, 0)},
};

const TMap<FString, double> calorificValueMap = {
	{TEXT("Andunie"), 22.85 }, // sunset treetop, dummy FColor value
	{TEXT("Carne"), 6.70 },
	{TEXT("Luine"), 11.30 },
	{TEXT("Laiqua"), 1.74 },
	{TEXT("Laure"), 5.63 },
	{TEXT("Helin"), 2.58 },
	{TEXT("Culuina"), 5.42 },
};

const TMap<FString, double> sizeMap = {
	{TEXT("Lung"), 1},
	{TEXT("Nica"), 0.2},
	{TEXT("Alta"), 1.75},
	{TEXT("Polda"), 2.25},
};
