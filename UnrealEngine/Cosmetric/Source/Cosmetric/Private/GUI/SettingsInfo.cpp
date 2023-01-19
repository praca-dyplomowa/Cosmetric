// Fill out your copyright notice in the Description page of Project Settings.


#include "GUI/SettingsInfo.h"

USettingsInfo::USettingsInfo()
{
	SaveSlotName = TEXT("Settings");
	Resolution = FString("1920x1080");
	Windowed = false;
	Shadows = 2;
	RenderDistance = 4;
	Textures = 2;
	ResolutionScaling = 100;
	AntiAliasing = 2;
	PostProcessing = 2;
	Tutorials = true;
}
