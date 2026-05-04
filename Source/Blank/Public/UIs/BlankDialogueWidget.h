#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "BlankDialogueWidget.generated.h"

class UTextBlock;

UCLASS()
class BLANK_API UBlankDialogueWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDialogueText(const FText& InText); // TODO: 構造体なのでアドレスを渡す、という理解でいいかチェック

protected:
	// このC++をベースにBPを作ったとき、同名コンポーネントが配置されていない場合エラーにするという制約
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;
};
