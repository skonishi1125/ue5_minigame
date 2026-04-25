#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class USoundBase;

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

	// 浮遊処理
	UFUNCTION(BlueprintPure)
	float TransformedSin();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float Amplitude = .1f; // 振幅
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
	float TimeConstant = 2.f; // 波長 値が大きくなるほど往復スピードが上がる。(波長 = 2π / k における、k)

	// 回転処理
	void RotateObject(float DeltaTime);
	float RotationSpeed = 90.0f; // [deg/s]: 秒間あたり何度回るか

private:
	void SetupComponents();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RunningTime;

	// コインに触れた時の処理
	UFUNCTION()
	void OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractArea;

	// sfx
	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundBase* PickupSound;
	bool bIsPicked = false; // 重複取得を防ぐフラグ

};
