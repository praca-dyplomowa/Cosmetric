// Fill out your copyright notice in the Description page of Project Settings.


#include "Tree/OakLikeTree.h"

AOakLikeTree::AOakLikeTree()
{
	Name = TEXT("OakLike tree");
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

void AOakLikeTree::RenderTreetop(double offset)
{
	auto translation = FVector(0, 0, TrunkRender.Height + GetMeshOffset(TrunkInit.StaticMesh, TrunkRender.SegmentSize));
	if (TreetopRender.Instanced &&
		TreetopRender.Instanced->GetStaticMesh() &&
		TreetopRender.Instanced->GetMaterial(0)) {
		auto meshSize = TreetopRender.Instanced->GetStaticMesh()->GetBoundingBox().GetSize();

		auto rotation =  360 / PartsOfCircle;
		auto ZAxis = FVector(0, 0, 1);
		auto radius = InitialTreetopRadius;

		auto segmentHeight = TreetopRender.SegmentSize * offset;
		auto segmentNumber = round((TreetopRender.Height - TreetopRender.SegmentSize) / segmentHeight) + 1;
		auto segmentScale = TreetopRender.SegmentSize / meshSize.Z;


		TArray<FTransform> transforms;
		transforms.Empty(segmentNumber * PartsOfCircle + 1);
		TreetopRender.Instanced->ClearInstances();

		// base of the treetop
		transforms.Add(
			FTransform(
				FRotator(0, 0, 0),
				translation,
				FVector(
					radius * 2 / meshSize.X,
					radius * 2 / meshSize.Y,
					10 / meshSize.Z
				)
			)
		);
		for (int i = 0; i < segmentNumber; i++) {
			for (int rot = 0; rot < 360; rot += rotation) {
				auto thisTranslation = translation + FVector(radius, 0, 0).RotateAngleAxis(rot, ZAxis);
				auto rotator = FRotator(
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance),
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance),
					Stream.FRandRange(-TreetopRender.RotationDegreesVariance, TreetopRender.RotationDegreesVariance)
				);
				auto scale = FVector(
					TreetopRender.HorizontalStretch * Stream.FRandRange(1 - TreetopRender.HorizontalScalingVariance, 1 + TreetopRender.HorizontalScalingVariance),
					TreetopRender.HorizontalStretch * Stream.FRandRange(1 - TreetopRender.HorizontalScalingVariance, 1 + TreetopRender.HorizontalScalingVariance),
					1
				) * segmentScale;
				transforms.Add(FTransform(
					rotator,
					thisTranslation,
					scale
				));
			}
			radius *= (1 - ShrinkFactor);
			translation += FVector(0, 0, segmentHeight);
		}
		TreetopRender.Instanced->AddInstances(transforms, false);
	}
}
