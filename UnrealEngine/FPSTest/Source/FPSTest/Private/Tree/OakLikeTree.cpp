// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/OakLikeTree.h"

AOakLikeTree::AOakLikeTree()
{
	Name = TEXT("OakLike tree");
	TrunkInit.HeightBounds = FVector2D(600, 800);
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

	TreetopInit.HeightBounds = FVector2D(500, 800);
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

double AOakLikeTree::RenderTreetop(double offset)
{
	TreetopRender.Instanced->SetRelativeLocation(FVector(
		0,
		0,
		TrunkRender.Height - TreetopRender.Height + TreetopRender.SegmentSize + GetMeshOffset(TreetopInit.StaticMesh, TreetopRender.SegmentSize)
	));
	return RenderConeTreetop(offset, InitialTreetopRadius, PartsOfCircle);
}
