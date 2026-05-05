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

		CurrentDialogueIndex = 0;
	}

}


void ABlankPlayerController::ShowDialogue(const TArray<FText>& Messages)
{
	if (DialogueWidgetInstance && !bIsDialogueOpen)
	{
		// Messages.Num() > 0 でもよいので、空配列かどうかをチェックすることを忘れない
		if (Messages.IsEmpty())
		{
			UE_LOG(LogTemp, Error, TEXT("ABlankPlayerController::ShowDialogue(): メッセージ配列が空です。"));
			return;
		}

		// 渡されたダイアログ配列をControllerで保管
		CurrentDialogues = Messages;
		CurrentDialogueIndex = 0;

		DialogueWidgetInstance->SetDialogueText(Messages[CurrentDialogueIndex]);
		DialogueWidgetInstance->SetVisibility(ESlateVisibility::Visible);
		bIsDialogueOpen = true;

		// 仮に看板を呼んでいる途中にマウス操作などを弾きたかったら、下記の通り記載する
		//FInputModeGameAndUI InputMode; // GameAndUI とすることで、Eキーへの入力を受けつつUI操作も可能とさせる
		//InputMode.SetWidgetToFocus(DialogueWidgetInstance->TakeWidget()); // 開いたUIにフォーカス設定をする
		//InputMode.SetHideCursorDuringCapture(false); // クリック時にカーソルを隠さないようにする
		//SetInputMode(InputMode);

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

void ABlankPlayerController::ProceedDialogue()
{
	// テキストが開いていないとき
	if (!bIsDialogueOpen)
	{
		return;
	}

	CurrentDialogueIndex++;
	if (CurrentDialogues.IsValidIndex(CurrentDialogueIndex))
	{
		DialogueWidgetInstance->SetDialogueText(CurrentDialogues[CurrentDialogueIndex]);
	}
	else
	{
		CloseDialogue();
	}

}

void ABlankPlayerController::CloseDialogue()
{
	if (DialogueWidgetInstance && bIsDialogueOpen)
	{
		// 入力モードを戻す
		//FInputModeGameOnly InputMode;
		//SetInputMode(InputMode);

		DialogueWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
		bIsDialogueOpen = false;
		CurrentDialogues.Empty();
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
