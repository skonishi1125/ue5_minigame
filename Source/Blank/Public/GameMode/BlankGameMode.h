#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BlankGameMode.generated.h"

UCLASS()
class BLANK_API ABlankGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	void CheckWinCondition(int32 CurrentCoinCount);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Rules")
	int32 TargetCoinCount = 10;

	// BlueprintImplementableEvent: C++ で呼ぶが、「CLEAR」などUIを出す処理はBP側でするということ
	UFUNCTION(BlueprintImplementableEvent, Category = "Rules")
	void OnGameCleared();

private:
	UFUNCTION()
	void HandleCoinCountChanged(int32 NewCoinCount);

};
