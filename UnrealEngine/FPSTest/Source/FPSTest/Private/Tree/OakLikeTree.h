// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tree/GenericTree.h"
#include "OakLikeTree.generated.h"

/**
 * 
 */
UCLASS()
class AOakLikeTree : public AGenericTree
{
	GENERATED_BODY()
public: 
	AOakLikeTree();
protected:
	virtual void RenderTreetop(double offset) override;
	double ShrinkFactor = 0.15;
	double InitialTreetopRadius = 300;
	int PartsOfCircle = 7;
};
