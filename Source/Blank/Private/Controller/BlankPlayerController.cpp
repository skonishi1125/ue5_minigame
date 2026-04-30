#include "Controller/BlankPlayerController.h"
#include "Camera/BlankCameraManager.h"

ABlankPlayerController::ABlankPlayerController()
{
	// GameModeにこの自作Controllerを使うように指定する
	// このControllerでは、下記の自作カメラ設定を使うようにする
	// ABlankCameramanagerでは、カメラの比率などを持たせてゲーム共通のルールとしている
	PlayerCameraManagerClass = ABlankCameraManager::StaticClass();
}

// Character -> 何かに遷移するとき
void ABlankPlayerController::PossessToNewPawn(APawn* TargetPawn)
{
	if (TargetPawn)
	{
		OriginalPawn = GetPawn();
		Possess(TargetPawn);
	}
}

// 何か -> Character に戻るとき
void ABlankPlayerController::ReturnToOriginalPawn()
{
	if (OriginalPawn)
	{
		Possess(OriginalPawn);
		OriginalPawn = nullptr;
	}
}
