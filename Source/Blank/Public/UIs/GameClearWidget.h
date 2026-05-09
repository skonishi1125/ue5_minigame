#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"

class UButton;
class UWidgetAnimation;

UCLASS()
class BLANK_API UGameClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void PlayFadeInAnimation();

	UFUNCTION()
	void OnRetryButtonClicked();

protected:
	virtual void NativeConstruct() override;

	// エディタで同名のanimationを作成すると、自動紐づけされるようにする
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(meta = (BindWidget))
	UButton* RetryButton;

};
