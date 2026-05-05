#include "UIs/BlankDialogueWidget.h"
#include "Components/TextBlock.h"

void UBlankDialogueWidget::SetDialogueText(const FText& InText)
{
	//if (MessageText)
	//{
	//	MessageText->SetText(InText);
	//}

	if (!MessageText) return;

	GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);

	// FText は 1文字ずつ切り出せないので、FString にして処理を行う
	FullString = InText.ToString();
	CurrentCharIndex = 0;
	bIsTyping = true;

	MessageText->SetText(FText::GetEmpty()); // 最初の空っぽ状態をセット

	GetWorld()->GetTimerManager().SetTimer(
		TypewriterTimerHandle,
		this,
		&UBlankDialogueWidget::UpdateTyping, // この関数を
		TypewriterSpeed, // この間隔で呼ぶ
		true // ループ有効
	);
}

void UBlankDialogueWidget::UpdateTyping()
{
	CurrentCharIndex++;

	// index 分だけ文字列から切り出して格納
	FString CurrentString = FullString.Left(CurrentCharIndex);
	// FString で加工したデータを FText に戻してセット
	MessageText->SetText(FText::FromString(CurrentString));

	if (CurrentCharIndex >= FullString.Len())
	{
		SkipTyping();
	}

}

void UBlankDialogueWidget::SkipTyping()
{
	// タイマーを停止し、全文を一気に表示する
	GetWorld()->GetTimerManager().ClearTimer(TypewriterTimerHandle);
	MessageText->SetText(FText::FromString(FullString));
	bIsTyping = false;
}
