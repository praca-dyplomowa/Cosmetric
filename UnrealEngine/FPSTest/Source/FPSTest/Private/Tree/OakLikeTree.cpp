// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/OakLikeTree.h"

AOakLikeTree::AOakLikeTree()
{
	Species = TEXT("Alma");
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

void AOakLikeTree::SunsetizeTreetop(double segmentNum)
{
	auto segmentNumber = (segmentNum - 1) / PartsOfCircle + 1;
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
}
