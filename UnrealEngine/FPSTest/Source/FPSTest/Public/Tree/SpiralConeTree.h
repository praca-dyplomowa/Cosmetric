// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree/GenericTree.h"
#include "SpiralConeTree.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API ASpiralConeTree : public AGenericTree
{
	GENERATED_BODY()
public:
	ASpiralConeTree();
protected:
	virtual double RenderTrunk(double Offset) override;
	virtual double RenderTreetop(double Offset) override;

};
