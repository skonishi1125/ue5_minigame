#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UButton;

UCLASS()
class BLANK_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnStartButtonClicked();

	// UMG 上で、StartButton という名前で、`Button`型のものと紐づける
	// 存在しなければBP側でコンパイルが通らない
	UPROPERTY(meta = (BindWidget))
	UButton* StartButton;

	UPROPERTY(EditAnywhere, Category = "LevelTransition", meta = (AllowedClasses = "World"))
	TSoftObjectPtr<UWorld> NextLevel;

};
