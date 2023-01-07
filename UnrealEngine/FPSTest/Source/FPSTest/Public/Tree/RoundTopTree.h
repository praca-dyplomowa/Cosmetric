// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree/GenericTree.h"
#include "RoundTopTree.generated.h"

/**
 * 
 */
UCLASS()
class FPSTEST_API ARoundTopTree : public AGenericTree
{
	GENERATED_BODY()
public:
	ARoundTopTree();
protected:

	virtual double RenderTreetop(double Offset) override;
	virtual void SunsetizeTreetop(double segmentNum) override;
};
