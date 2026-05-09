#include "UIs/GameClearWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameClearWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ウィジェットに記載するボタンのクリックイベント（UE側が用意したもの）と割り当て
	if (RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UGameClearWidget::OnRetryButtonClicked);
	}
}

void UGameClearWidget::PlayFadeInAnimation()
{
	if (FadeInAnim)
	{
		PlayAnimation(FadeInAnim);
	}

	// 入力を切ってマウスに
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		FInputModeUIOnly InputMode;
		InputMode.SetWidgetToFocus(TakeWidget());
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = true;
	}

}

// 現在のレベルをもう一度開く
void UGameClearWidget::OnRetryButtonClicked()
{
	FString CurrentLevelName = UGameplayStatics::GetCurrentLevelName(this, true);
	UGameplayStatics::OpenLevel(this, FName(*CurrentLevelName));
}


