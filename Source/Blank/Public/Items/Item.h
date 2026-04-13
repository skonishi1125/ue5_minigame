#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BLANK_API AItem : public AActor
{
	GENERATED_BODY()
public:
	AItem();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
	// ルートとなる基準点（座標と回転のデータのみを持つ）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	// UPROPERTY: UE5のGabage Collection 対象下とする
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	// 回転処理
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = .25f; // 振幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 5.f; // RunningTimeにかける値 2π / k における、K

private:
	void SetupComponents();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;


};
