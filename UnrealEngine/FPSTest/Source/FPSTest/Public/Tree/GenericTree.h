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

UCLASS()
class FPSTEST_API AGenericTree : public AIsCollectable
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
	double RenderRoundTreetop(double Offset);
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

private:
	double InitStruct(FTreeComponentInit& init, FTreeComponentRender& render);
};
