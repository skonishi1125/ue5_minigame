// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/BlankCameraManager.h"

ABlankCameraManager::ABlankCameraManager()
{
}

void ABlankCameraManager::UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime)
{
	Super::UpdateViewTarget(OutVT, DeltaTime);

	// カメラ比率の決定
	OutVT.POV.bConstrainAspectRatio = true;
	OutVT.POV.AspectRatio = 16.0f / 9.0f;
}
