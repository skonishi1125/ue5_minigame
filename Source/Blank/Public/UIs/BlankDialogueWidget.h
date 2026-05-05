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
	// 値で渡すとコピー処理が発生する
	// FText はローカライズ用キーや履歴データを持つ重い構造体のため参照渡しで渡す
	void SetDialogueText(const FText& InText);
	bool IsTyping() const { return bIsTyping; }
	void SkipTyping();
protected:
	// このC++をベースにBPを作ったとき、同名コンポーネントが配置されていない場合エラーにするという制約
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> MessageText;
private:
	void UpdateTyping();

	UPROPERTY(EditAnywhere, Category = "Dialogue")
	float TypewriterSpeed = 0.05f;

	FTimerHandle TypewriterTimerHandle;

	FString FullString;

	int32 CurrentCharIndex = 0;

	bool bIsTyping = false;
};
