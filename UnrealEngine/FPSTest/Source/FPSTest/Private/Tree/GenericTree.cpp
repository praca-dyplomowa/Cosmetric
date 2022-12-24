// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/GenericTree.h"
#include "GUI/MyGameInstance.h"
#include "PerlinNoise.h"
#include "Math/RandomStream.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"
#include "UObject/UObjectGlobals.h"

#include <cmath>
#define GOLDEN_RATIO_ANGLE 137.5

class UInstancedStaticMeshComponent;

// Sets default values
AGenericTree::AGenericTree()
{
	Name = TEXT("Drzewo");
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Seed = 0;
	PrimaryActorTick.bCanEverTick = true;
	TrunkInit.InitialSegmentSize = 20.0;
	TrunkRender.RotationDegreesVariance = 15;
	TrunkRender.HorizontalScalingVariance = 0.05;
	TrunkInit.HeightBounds = FVector2D(100, 200);

	auto trunkStaticMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (trunkStaticMesh.Succeeded())  TrunkInit.StaticMesh = trunkStaticMesh.Object;
	TrunkInit.Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Wood_Walnut"));

	TreetopInit.InitialSegmentSize = 100.0;
	TreetopInit.VerticalOffsetPercentClamp = FVector2D(0.3, 0.7);
	TreetopRender.RotationDegreesVariance = 5;
	TreetopRender.HorizontalScalingVariance = 0.1;
	TreetopInit.HeightBounds = FVector2D(150, 300);

	auto treetopStaticMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Cone.Cone'"));
	if (treetopStaticMesh.Succeeded())  TreetopInit.StaticMesh = treetopStaticMesh.Object;
	TreetopInit.Material = LoadObject<UMaterial>(nullptr, TEXT("/Game/StarterContent/Materials/M_Tech_Hex_Tile"));



	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	TrunkRender.Instanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Trunk"));
	TreetopRender.Instanced = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Treetop"));

	TrunkRender.Instanced->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	TreetopRender.Instanced->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

}

void AGenericTree::Initialize(FTreetopInit treetopInit, FTrunkInit trunkInit) {
	TrunkInit = trunkInit;
	TreetopInit = treetopInit;
}

void AGenericTree::Initialize(FTreetopRenderVariables treetopRender, FTrunkRenderVariables trunkRender)
{
	TrunkRender.HorizontalStretch = trunkRender.HorizontalStretch;
	TrunkRender.RotationDegreesVariance = trunkRender.RotationDegreesVariance;
	TrunkRender.HorizontalScalingVariance = trunkRender.HorizontalScalingVariance;

	TreetopRender.HorizontalStretch = treetopRender.HorizontalStretch;
	TreetopRender.RotationDegreesVariance = treetopRender.RotationDegreesVariance;
	TreetopRender.HorizontalScalingVariance = treetopRender.HorizontalScalingVariance;
}

void AGenericTree::Initialize(FTrunkRenderVariables trunkRender, FTreetopRenderVariables treetopRender)
{
	this->Initialize(treetopRender, trunkRender);
}

void AGenericTree::Initialize(FTrunkInit trunkInit, FTreetopInit treetopInit, FTreetopRenderVariables treetopRender, FTrunkRenderVariables trunkRender) {
	this->Initialize(treetopInit, trunkInit);
	this->Initialize(treetopRender, trunkRender);
}


double AGenericTree::GetRandomFromVector(FVector2D& bounds)
{
	return Stream.FRandRange(bounds.X, bounds.Y);
}

// get distance from bottom of a mesh (in scale) to center (render location)
double AGenericTree::GetMeshOffset(UStaticMesh* staticMesh, double size)
{
	if (staticMesh == nullptr) {
		return 0;
	}
	auto bounds = staticMesh->GetBoundingBox();
	return (-bounds.Min.Z * size) / bounds.GetSize().Z;
}

// Called when the game starts or when spawned
void AGenericTree::BeginPlay()
{
	Super::BeginPlay();
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	if (GameInfo)
	{
		Seed = GameInfo->Seed;
	}
}

void AGenericTree::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGenericTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

double AGenericTree::RenderUp(FTreeComponentRender& generatedStruct, double Offset)
{
	if (generatedStruct.Instanced &&
		generatedStruct.Instanced->GetStaticMesh() &&
		generatedStruct.Instanced->GetMaterial(0)) {
		auto segmentHeight = generatedStruct.SegmentSize * Offset;
		auto segmentNumber = round((generatedStruct.Height - generatedStruct.SegmentSize) / segmentHeight) + 1;
		auto translation = FVector(0);
		auto segmentScale = generatedStruct.SegmentSize / generatedStruct.Instanced->GetStaticMesh()->GetBoundingBox().GetSize().Z;
		TArray<FTransform> transforms;
		transforms.Empty(segmentNumber);
		generatedStruct.Instanced->ClearInstances();
		for (int i = 0; i < segmentNumber; i++) {

			auto rotation = FRotator(
				Stream.FRandRange(-generatedStruct.RotationDegreesVariance, generatedStruct.RotationDegreesVariance),
				Stream.FRandRange(-generatedStruct.RotationDegreesVariance, generatedStruct.RotationDegreesVariance),
				Stream.FRandRange(-generatedStruct.RotationDegreesVariance, generatedStruct.RotationDegreesVariance)
			);
			auto scale = FVector(
				generatedStruct.HorizontalStretch * Stream.FRandRange(1 - generatedStruct.HorizontalScalingVariance, 1 + generatedStruct.HorizontalScalingVariance),
				generatedStruct.HorizontalStretch * Stream.FRandRange(1 - generatedStruct.HorizontalScalingVariance, 1 + generatedStruct.HorizontalScalingVariance),
				1
			) * segmentScale;
			transforms.Add(FTransform(
				rotation,
				translation,
				scale
			));
			translation += FVector(0, 0, segmentHeight);
		}
		generatedStruct.Instanced->AddInstances(transforms, false);
		return segmentNumber;
	}
	return 0;
}

double AGenericTree::RenderTrunk(double offset)
{
	return RenderUp(
		TrunkRender,
		offset
	);
}

double AGenericTree::RenderTreetop(double offset)
{
	return RenderUp(
		TreetopRender,
		offset
	);
}

void AGenericTree::OnConstruction(const FTransform& transform)
{

	Stream = FRandomStream(
		Seed
		+ GetActorLocation().X
		+ GetActorLocation().Y
		+ GetActorLocation().Z
	);
	Materials[0] = Stream.GetFraction() * 20;
	Materials[1] = Stream.GetFraction() * 30;
	
	double treetopOffset = InitStruct(TreetopInit, TreetopRender);
	double trunkOffset = InitStruct(TrunkInit, TrunkRender);

	auto spawnOffset = FVector(0, 0, GetMeshOffset(TrunkInit.StaticMesh, TrunkRender.SegmentSize));
	TrunkRender.Instanced->SetRelativeLocation(spawnOffset);

	spawnOffset.Z = TrunkRender.Height + GetMeshOffset(TreetopInit.StaticMesh, TreetopRender.SegmentSize);
	TreetopRender.Instanced->SetRelativeLocation(spawnOffset);

	RenderTrunk(trunkOffset);
	RenderTreetop(treetopOffset);
}

void AGenericTree::OnCollected()
{
	Super::OnCollected();
	SaveTreeDestroyed();
	Destroy();
}


void AGenericTree::SaveTreeDestroyed()
{
	auto GameInfo = ((UMyGameInstance*)(GetWorld()->GetGameInstance()))->GameInfo;
	auto chunkLocation = GetOwner()->GetActorLocation();
	bool didExist = true;
	auto addedTreeLocation = FVector2D(GetActorLocation());
	FChunkSaveInfo* thisChunk = GameInfo->ChunkInfo.Find(chunkLocation);
	if (thisChunk == nullptr) {
		thisChunk = new FChunkSaveInfo(); // create new object to store tree data for this tree's chunk
		didExist = false;
	}
	thisChunk->DestroyedTreePositions.Add(FVector2D(GetActorLocation())); // store this tree's XY position
			
	if (!didExist) {
		GameInfo->ChunkInfo.Add(chunkLocation, *thisChunk);
	}
}

double AGenericTree::RenderConeTreetop(double Offset, double InitialTreetopRadius, int PartsOfCircle, ShrinkType type)
{
	auto translation = FVector(0);
	if (TreetopRender.Instanced &&
		TreetopRender.Instanced->GetStaticMesh() &&
		TreetopRender.Instanced->GetMaterial(0)) {
		auto meshSize = TreetopRender.Instanced->GetStaticMesh()->GetBoundingBox().GetSize();

		auto rotation = 360 / PartsOfCircle;
		auto radius = InitialTreetopRadius;

		auto segmentHeight = TreetopRender.SegmentSize * Offset;
		auto segmentNumber = round((TreetopRender.Height - TreetopRender.SegmentSize) / segmentHeight) + 1;
		auto segmentScale = TreetopRender.SegmentSize / meshSize.Z;
		double shrink;
		if (type == ShrinkType::Linear) {
			shrink = InitialTreetopRadius / (segmentNumber - 1);
		}
		else {
			shrink = FMath::Pow( 0.05, 1 / FMath::Max((segmentNumber - 1), 1));
		}
		TArray<FTransform> transforms;
		transforms.Empty(segmentNumber * PartsOfCircle + 1);
		TreetopRender.Instanced->ClearInstances();
		TArray<double> angles;
		angles.Init(0, PartsOfCircle);
		for (int i = 1; i < PartsOfCircle; i++) {
			angles[i] = rotation * i;
		}
		auto scale = FVector(
			TreetopRender.HorizontalStretch * Stream.FRandRange(1 - TreetopRender.HorizontalScalingVariance, 1 + TreetopRender.HorizontalScalingVariance),
			TreetopRender.HorizontalStretch * Stream.FRandRange(1 - TreetopRender.HorizontalScalingVariance, 1 + TreetopRender.HorizontalScalingVariance),
			1
		) * segmentScale;

		auto bonusAngle = 0.0f;
		for (int i = 0; i < segmentNumber - 1; i++) {
			for (int rot = 0; rot < PartsOfCircle; rot++) {
				auto thisTranslation = translation + FVector(radius, 0, 0).RotateAngleAxis(angles[rot] + bonusAngle  + Stream.RandRange(-4, 4), FVector::ZAxisVector);
				auto rotator = FRotator(
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance),
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance),
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance)
				);
				
				transforms.Add(FTransform(
					rotator,
					thisTranslation,
					scale
				));
			}
			bonusAngle += GOLDEN_RATIO_ANGLE;
			if (type == ShrinkType::Linear) {
				radius -= shrink;
			}
			else {
				radius *= shrink;
			}
			translation += FVector(0, 0, segmentHeight);
		}
		transforms.Add(
			FTransform(
				FRotator(0, 0, 0),
				translation,
				scale
			)
		);
		TreetopRender.Instanced->AddInstances(transforms, false);
		return segmentNumber;
	}
	return 0;
}

double AGenericTree::RenderSpiralTrunk(double Offset, double SpiralRadius, double RotationDegree, ShrinkType type)
{
	auto translation = FVector(0);
	if (TrunkRender.Instanced &&
		TrunkRender.Instanced->GetStaticMesh() &&
		TrunkRender.Instanced->GetMaterial(0)) {
		auto meshSize = TrunkRender.Instanced->GetStaticMesh()->GetBoundingBox().GetSize();

		auto ZAxis = FVector(0, 0, 1);
		auto radius = SpiralRadius;
		auto rotation = 0;
		auto segmentHeight = TrunkRender.SegmentSize * Offset;
		auto segmentNumber = round((TrunkRender.Height - TrunkRender.SegmentSize) / segmentHeight) + 1;
		auto segmentScale = TrunkRender.SegmentSize / meshSize.Z;

		double shrink;
		if (type == ShrinkType::Linear) {
			shrink = SpiralRadius / (segmentNumber - 1);
		}
		else {
			shrink = FMath::Pow(0.1, 1 / FMath::Max((segmentNumber - 1), 1));
		}

		TArray<FTransform> transforms;
		transforms.Empty(segmentNumber);
		TrunkRender.Instanced->ClearInstances();

		// base of the Trunk
		for (int i = 0; i < segmentNumber - 1; i++) {
			auto thisTranslation = translation + FVector(radius, 0, 0).RotateAngleAxis(rotation, ZAxis);
			auto rotator = FRotator(
				Stream.FRandRange(-TrunkRender.RotationDegreesVariance, TrunkRender.RotationDegreesVariance),
				Stream.FRandRange(-TrunkRender.RotationDegreesVariance, TrunkRender.RotationDegreesVariance),
				Stream.FRandRange(-TrunkRender.RotationDegreesVariance, TrunkRender.RotationDegreesVariance)
			);
			auto scale = FVector(
				TrunkRender.HorizontalStretch * Stream.FRandRange(1 - TrunkRender.HorizontalScalingVariance, 1 + TrunkRender.HorizontalScalingVariance),
				TrunkRender.HorizontalStretch * Stream.FRandRange(1 - TrunkRender.HorizontalScalingVariance, 1 + TrunkRender.HorizontalScalingVariance),
				1
			) * segmentScale;
			transforms.Add(FTransform(
				rotator,
				thisTranslation,
				scale
			));
			translation += FVector(0, 0, segmentHeight);
			rotation += RotationDegree;
			if (type == ShrinkType::Linear) {
				radius -= shrink;
			}
			else {
				radius *= shrink;
			}
		}
		transforms.Add(
			FTransform(
				FRotator(0, 0, 0),
				translation,
				FVector(
					TrunkRender.HorizontalStretch * Stream.FRandRange(1 - TrunkRender.HorizontalScalingVariance, 1 + TrunkRender.HorizontalScalingVariance),
					TrunkRender.HorizontalStretch * Stream.FRandRange(1 - TrunkRender.HorizontalScalingVariance, 1 + TrunkRender.HorizontalScalingVariance),
					1
				) * segmentScale
			)
		);

		TrunkRender.Instanced->AddInstances(transforms, false);
		return segmentNumber;
	}
	return 0;
}

TArray<FVector> AGenericTree::PrepareSunsetVectors(FVector beginning, FVector end, int number)
{
	auto Lerp = [](double start, double end, double delta) {
		return start + delta * (end - start);
	};
	TArray<FVector> colors;
	colors.Empty(number);
	colors.Add(beginning);
	if(number <= 1) return colors;
	for (int i = 1; i <= number - 2; i++) {
		double delta = i / (number - 1.0f);
		colors.Add(FVector(
			Lerp(beginning.X, end.X, delta),
			Lerp(beginning.Y, end.Y, delta),
			Lerp(beginning.Z, end.Z, delta)
		));
	}
	colors.Add(end);
	auto num = colors.Num();
	return colors;
}

double AGenericTree::InitStruct(FTreeComponentInit& init, FTreeComponentRender& render)
{
	if (init.VerticalOffsetPercentClamp.X < 0) init.VerticalOffsetPercentClamp.X = 0;
	if (init.VerticalOffsetPercentClamp.Y > 1) init.VerticalOffsetPercentClamp.Y = 1;
	render.Height = GetRandomFromVector(init.HeightBounds);
	auto offset = GetRandomFromVector(init.VerticalOffsetPercentClamp);
	int desiredSegmentNumber = round((render.Height - init.InitialSegmentSize) / (offset * init.InitialSegmentSize));
	render.SegmentSize = render.Height / (offset * desiredSegmentNumber + 1);
	if (init.StaticMesh) {
		render.Instanced->SetStaticMesh(init.StaticMesh);
	}
	if (init.Material) {
		render.Instanced->SetMaterial(0, init.Material);
	}

	return offset;
}


