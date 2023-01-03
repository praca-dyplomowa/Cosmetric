// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/RoundTopTree.h"

ARoundTopTree::ARoundTopTree()
{
	Name = TEXT("RoundTop tree");
	TrunkInit.HeightBounds = FVector2D(200, 300);
	TrunkInit.InitialSegmentSize = 60;

	TrunkInit.Material = LoadObject<UMaterial>(
		nullptr,
		TEXT("/Game/Trees/Materials/M_Oak_Bark")
		);
	TrunkInit.StaticMesh = LoadObject<UStaticMesh>(
		nullptr,
		TEXT("/Game/StarterContent/Shapes/HexTile_mesh")
		);

	TrunkRender.HorizontalStretch = 0.1;
	TrunkRender.RotationDegreesVariance = 15;
	TrunkRender.HorizontalScalingVariance = 0.2;

	TreetopInit.HeightBounds = FVector2D(300, 500);
	TreetopInit.InitialSegmentSize = 100;
	TreetopInit.VerticalOffsetPercentClamp = FVector2D(0.5, 0.8);

	auto treetopStaticMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (treetopStaticMesh.Succeeded())  TreetopInit.StaticMesh = treetopStaticMesh.Object;
	TreetopInit.Material = LoadObject<UMaterial>(
		nullptr,
		TEXT("/Game/Trees/Materials/M_Oak_Treetop")
		);

	TreetopRender.HorizontalStretch = 2.3;
	TreetopRender.RotationDegreesVariance = 5;
	TreetopRender.HorizontalScalingVariance = 0.3;
}

double ARoundTopTree::RenderTreetop(double Offset)
{
	return RenderRoundTreetop(Offset, 7);
}
