// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/GameSaveInfo.h"
 
FBuildingInfo::FBuildingInfo(const FTransform& transform, TSubclassOf<ABuildingBase> myClass) {
	Transform = transform;
	Class = myClass;
};

FBuildingInfo::FBuildingInfo() {

}

UGameSaveInfo::UGameSaveInfo() {
	SaveSlotName = TEXT("New Save Slot");
	UserIndex = 0;
}
