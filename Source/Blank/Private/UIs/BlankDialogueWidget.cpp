#include "UIs/BlankDialogueWidget.h"
#include "Components/TextBlock.h"

void UBlankDialogueWidget::SetDialogueText(const FText& InText)
{
	if (MessageText)
	{
		MessageText->SetText(InText);
	}
}
