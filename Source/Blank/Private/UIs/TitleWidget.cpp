#include "UIs/TitleWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

// UI が画面生成されたときに一度だけ呼ばれる処理
void UTitleWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UTitleWidget::OnStartButtonClicked);
	}

}

void UTitleWidget::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(this, NextLevelName);
}
