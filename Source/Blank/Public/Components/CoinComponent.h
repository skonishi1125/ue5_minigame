#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CoinComponent.generated.h"

 //クラスの定義を展開するマクロ
 //コイン枚数変化を示すデリゲートの宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinCountChanged, int32, NewCoinCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UCoinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCoinComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// コイン取得時に外部から呼ぶため、public とする
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetCoinCount() const;

	// マクロで定義したクラスを使用し、
	// Blueprint 側からイベントを受け取るための関数
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCoinCountChanged OnCoinCountChanged;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int32 CoinCount = 0;
};
