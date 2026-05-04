#include "Controller/BlankPlayerController.h"
#include "Camera/BlankCameraManager.h"
#include "UIs/BlankDialogueWidget.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

ABlankPlayerController::ABlankPlayerController()
{
	// GameModeにこの自作Controllerを使うように指定する
	// このControllerでは、下記の自作カメラ設定を使うようにする
	// ABlankCameramanagerでは、カメラの比率などを持たせてゲーム共通のルールとしている
	PlayerCameraManagerClass = ABlankCameraManager::StaticClass();
}

void ABlankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// TitleGameMode と同じフローで、UIを生成する
	if (DialogueWidgetClass)
	{
		// GetWorld()でなく、thisでよい
		// UIのオーナーとして世界全体を渡しても良いが、Controller を渡すことで、そのPlayerにしか見えないUIであることを示す
		DialogueWidgetInstance = CreateWidget<UBlankDialogueWidget>(this, DialogueWidgetClass);
		if (DialogueWidgetInstance)
		{
			// ゲーム開始時、Hidden 状態でUIを生成しておく
			DialogueWidgetInstance->AddToViewport();
			DialogueWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		}
	}

}


void ABlankPlayerController::ShowDialogue(const FText& Message)
{
	if (DialogueWidgetInstance && !bIsDialogueOpen)
	{
		DialogueWidgetInstance->SetDialogueText(Message);
		DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		bIsDialogueOpen = true;

		FInputModeGameAndUI InputMode; // GameAndUI とすることで、Eキーへの入力を受けつつUI操作も可能とさせる
		InputMode.SetWidgetToFocus(DialogueWidgetInstance->TakeWidget()); // 開いたUIにフォーカス設定をする
		InputMode.SetHideCursorDuringCapture(false); // クリック時にカーソルを隠さないようにする

		SetInputMode(InputMode);

		// ABlankCharacter でなく, 親の ACharacter でキャストする
		// ファイル先頭で include して ABlankCharacter を読み込む必要が無くなる
		// 結果、自身のプロジェクト独自クラスを知らなくてもコンパイルが通る
		// (将来別のキャラクタークラスでダイアログを出したいときもこちらで問題なくなる)
		if (ACharacter* CastCharacter = Cast<ACharacter>(GetPawn()))
		{
			CastCharacter->GetCharacterMovement()->StopMovementImmediately();
			FlushPressedKeys(); // 押しっぱなし判定などもリフレッシュ
		}

		SetIgnoreMoveInput(true); // テキスト表示中は移動操作を無効化

	}
}

void ABlankPlayerController::CloseDialogue()
{
	if (DialogueWidgetInstance && bIsDialogueOpen)
	{
		DialogueWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		bIsDialogueOpen = false;

		// 入力モードを戻す
		FInputModeGameOnly InputMode;
		SetInputMode(InputMode);

		SetIgnoreMoveInput(false);
	}
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
