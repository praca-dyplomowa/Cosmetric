// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/RoundTopTree.h"

ARoundTopTree::ARoundTopTree()
{
	Species = TEXT("Galad");
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

	TreetopInit.StaticMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
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

void ARoundTopTree::SunsetizeTreetop(double segmentNum)
{
	int PartsOfCircle = 7;
	auto segmentNumber = (segmentNum - 2) / PartsOfCircle ;
	auto sunsetColors = PrepareSunsetVectors(FVector(0.209919, 0, 0.651), FVector(0.6146, 0.1105, 0), segmentNumber + 2);
	TArray<float> data;
	data.Init(0, 3);
	data[0] = (float)sunsetColors[0].X;
	data[1] = (float)sunsetColors[0].Y;
	data[2] = (float)sunsetColors[0].Z;
	TreetopRender.Instanced->SetCustomData(0, data, false);

	for (int i = 1; i <= segmentNumber; i++) {
		auto msg = sunsetColors[i].ToString();
		data[0] = (float)sunsetColors[i].X;
		data[1] = (float)sunsetColors[i].Y;
		data[2] = (float)sunsetColors[i].Z;
		for (int j = 1; j <= PartsOfCircle; j++) {
			TreetopRender.Instanced->SetCustomData((i - 1) * PartsOfCircle + j, data, false);
		}
	}
	data[0] = (float)sunsetColors[segmentNumber + 1].X;
	data[1] = (float)sunsetColors[segmentNumber + 1].Y;
	data[2] = (float)sunsetColors[segmentNumber + 1].Z;

	TreetopRender.Instanced->SetCustomData(segmentNumber * PartsOfCircle + 1, data, false);
	TreetopRender.Instanced->MarkRenderStateDirty();
}