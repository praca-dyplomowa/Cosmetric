// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/SpiralConeTree.h"

ASpiralConeTree::ASpiralConeTree()
{
	Name = TEXT("SpiralCone tree");
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

	auto treetopStaticMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	if (treetopStaticMesh.Succeeded())  TreetopInit.StaticMesh = treetopStaticMesh.Object;
	TreetopInit.Material = LoadObject<UMaterial>(
		nullptr,
		TEXT("/Game/Trees/Materials/M_Oak_Treetop")
		);

	TreetopRender.HorizontalStretch = 1;
	TreetopRender.RotationDegreesVariance = 0;
	TreetopRender.HorizontalScalingVariance = 0;
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
	int PartsOfCircle = 7;
	auto segmentNumber = (RenderConeTreetop(Offset, 300, PartsOfCircle) - 1) / PartsOfCircle;
	auto sunsetColors = PrepareSunsetVectors(FVector(0.209919, 0, 0.651), FVector(0.6146, 0.1105, 0), segmentNumber + 1);
	TArray<float> data;
	data.Init(0, 3);
	for (int i = 0; i < segmentNumber; i++) {
		auto msg = sunsetColors[i].ToString();
		data[0] = (float)sunsetColors[i].X;
		data[1] = (float)sunsetColors[i].Y;
		data[2] = (float)sunsetColors[i].Z;
		for (int j = 0; j < PartsOfCircle; j++) {
			TreetopRender.Instanced->SetCustomData(i * PartsOfCircle + j, data, false);
		}
	}
	data[0] = (float)sunsetColors[segmentNumber].X;
	data[1] = (float)sunsetColors[segmentNumber].Y;
	data[2] = (float)sunsetColors[segmentNumber].Z;

	TreetopRender.Instanced->SetCustomData(segmentNumber * PartsOfCircle, data, false);
	TreetopRender.Instanced->MarkRenderStateDirty();
	TreetopRender.Instanced->SetRelativeLocation(FVector(
		-150,
		0,
		TrunkRender.Height - TreetopRender.Height + TreetopRender.SegmentSize + GetMeshOffset(TreetopInit.StaticMesh, TreetopRender.SegmentSize)
	));
	CustomizedColors = true;
	return segmentNumber * PartsOfCircle + 1;
	
}
