#pragma once

#include "CoreMinimal.h"
#include "Camera/PlayerCameraManager.h"
#include "BlankCameraManager.generated.h"

UCLASS()
class BLANK_API ABlankCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ABlankCameraManager();
	virtual void UpdateViewTarget(FTViewTarget& OutVT, float DeltaTime) override; // カメラ座標, 回転等を決定する関数

};
