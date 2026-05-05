#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlankPlayerController.generated.h"

class APawn;
class UBlankDialogueWidget;

UCLASS()
class BLANK_API ABlankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABlankPlayerController();
	void PossessToNewPawn(APawn* TargetPawn); // C++の別クラスから呼び出す想定だけなら、UFUNCTION等は不要
	void ReturnToOriginalPawn();

	// ======== ダイアログUI関連 ========
	void ShowDialogue(const TArray<FText>& Messages);
	void ProceedDialogue();
	void CloseDialogue();
	bool IsDialogueOpen() const { return bIsDialogueOpen; }
protected:
	virtual void BeginPlay() override;
private:

	// APawn* OriginalPawn でも同じだが、
	// UE5 におけるクラスメンバ変数は TObjectPtr を作るこちらが好ましい
	// エディタ起動時間の短縮、メモリ使用量の削減ができる
	UPROPERTY()
	TObjectPtr<APawn> OriginalPawn;

	// ======== ダイアログUI関連 ========
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UBlankDialogueWidget> DialogueWidgetClass;

	UPROPERTY()
	TObjectPtr<UBlankDialogueWidget> DialogueWidgetInstance;

	UPROPERTY()
	TArray<FText> CurrentDialogues;

	int32 CurrentDialogueIndex = 0;
	bool bIsDialogueOpen = false;


};
