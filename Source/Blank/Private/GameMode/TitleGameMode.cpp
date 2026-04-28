#include "GameMode/TitleGameMode.h"
#include "UIs/TitleWidget.h"
#include "Blueprint/UserWidget.h" // AddToViewport に必要

void ATitleGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* Controller = GetWorld()->GetFirstPlayerController();

	// Controller && エディタ側で TitleWidgetClass が設定されている場合
	if (Controller && TitleWidgetClass)
	{
		UTitleWidget* TitleWidget = CreateWidget<UTitleWidget>(GetWorld(), TitleWidgetClass);

		if (TitleWidget)
		{
			TitleWidget->AddToViewport();

			// 入力モードをUI専用にし、マウスカーソルを表示させる
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(TitleWidget->TakeWidget());
			Controller->SetInputMode(InputMode);
			Controller->bShowMouseCursor = true;
		}

	}

}
