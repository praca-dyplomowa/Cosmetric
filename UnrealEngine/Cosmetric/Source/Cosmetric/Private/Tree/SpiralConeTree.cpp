// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/SpiralConeTree.h"

ASpiralConeTree::ASpiralConeTree()
{
	Species = TEXT("Lore");
	PartsOfCircle = 7;
	TrunkInit.HeightBounds = FVector2D(800, 1200);
	TrunkInit.InitialSegmentSize = 60;

	TrunkInit.Material = LoadObject<UMaterial>(
		nullptr,
		TEXT("/Game/Trees/Materials/M_Oak_Bark")
		);
	TrunkInit.StaticMesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("/Game/StarterContent/Shapes/HexTile_mesh")
		);
	TrunkInit.VerticalOffsetPercentClamp = FVector2D(0.7, 0.9);
	TrunkRender.HorizontalStretch = 0.1;
	TrunkRender.RotationDegreesVariance = 15;
	TrunkRender.HorizontalScalingVariance = 0.2;

	TreetopInit.HeightBounds = FVector2D(500, 700);
	TreetopInit.InitialSegmentSize = 70;
	TreetopInit.VerticalOffsetPercentClamp = FVector2D(0.6, 0.8);

	TreetopInit.StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	TreetopInit.Material = LoadObject<UMaterial>(
		nullptr,
		TEXT("/Game/Trees/Materials/M_Oak_Treetop")
		);

	TreetopRender.HorizontalStretch = 1;
	TreetopRender.RotationDegreesVariance = 0;
	TreetopRender.HorizontalScalingVariance = 0;


	FoodAmountClamp = FVector2D(1, 10);
	WoodAmountClamp = FVector2D(5, 20);
}

double ASpiralConeTree::RenderTrunk(double Offset)
{
	auto spiralRadius = 150;
	auto spawnOffset = FVector(-spiralRadius, 0, GetMeshOffset(TrunkInit.StaticMesh, TrunkRender.SegmentSize));
	TrunkRender.Instanced->SetRelativeLocation(spawnOffset);
	return RenderSpiralTrunk(Offset, spiralRadius, 25);
}

double ASpiralConeTree::RenderTreetop(double Offset)
{
	auto num = Super::RenderTreetop(Offset);
	TreetopRender.Instanced->SetRelativeLocation(FVector(
		-150,
		0,
		TrunkRender.Height - TreetopRender.Height + TreetopRender.SegmentSize + GetMeshOffset(TreetopInit.StaticMesh, TreetopRender.SegmentSize)
	));
	return num;
}
