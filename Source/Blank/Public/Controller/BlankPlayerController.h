#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlankPlayerController.generated.h"

class APawn;

UCLASS()
class BLANK_API ABlankPlayerController : public APlayerController
{
	GENERATED_BODY()
private:

	// APawn* OriginalPawn でも同じだが、
	// UE5 におけるクラスメンバ変数は TObjectPtr を作るこちらが好ましい
	// エディタ起動時間の短縮、メモリ使用量の削減ができる
	UPROPERTY()
	TObjectPtr<APawn> OriginalPawn;
public:
	ABlankPlayerController();
	void PossessToNewPawn(APawn* TargetPawn);

	void ReturnToOriginalPawn();
};
