#include "Controller/BlankPlayerController.h"
#include "Camera/BlankCameraManager.h"

ABlankPlayerController::ABlankPlayerController()
{
	// GameModeにこの自作Controllerを使うように指定する
	// このControllerでは、下記の自作カメラ設定を使うようにする
	// ABlankCameramanagerでは、カメラの比率などを持たせてゲーム共通のルールとしている
	PlayerCameraManagerClass = ABlankCameraManager::StaticClass();
}
