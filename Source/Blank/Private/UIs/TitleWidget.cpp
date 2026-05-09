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
	if (!NextLevel.IsNull())
	{
		// ソフト参照を使ってレベルを開く
		// Packaging したとき、Cookerがアセットを検知できる）
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, NextLevel);
	}
}
