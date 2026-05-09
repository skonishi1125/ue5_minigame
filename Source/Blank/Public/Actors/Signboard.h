#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"

#include "Signboard.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

UENUM(BlueprintType)
enum class ESignboardState : uint8
{
	Initial,
	WaitingForCoins,
	Completed
};

UCLASS()
class BLANK_API ASignboard : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ASignboard();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(APawn* Interactor) override;

	UPROPERTY(EditInstanceOnly, Category = "Event")
	TObjectPtr<AActor> TargetActorToDestroy;

protected:
	virtual void BeginPlay() override;
private:
	// C++ からは継承不可だが、UE上からはアクセスできる
	// RO は、BPでメッシュを割り当てることは可能だが、ノードなどで置換する処理に制限をかけている状態
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SignboardMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> InteractArea;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MultiLine = "true"))
	//TArray<FConditionalDialogue> ConditionalDialogues;

	// allowprivateaccess 無しでもエラーは出ない
	// C++のコンパイラがポインタの宣言自体は許容する, 現在UE側でエラーは出ないという仕様
	// ただし今後のver upどで弾かれるようになりうるため、付けておく
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> InteractWidget;

	UFUNCTION()
	void OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, Category = "State")
	ESignboardState CurrentState = ESignboardState::Initial;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	TArray<FText> InitialTexts;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	TArray<FText> NotEnoughCoinsTexts;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	TArray<FText> SuccessTexts;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	TArray<FText> CompleteTexts;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	bool bHasMultipleText = false; // InitialTexts 以外を持つ場合

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	int32 RequiredCoinNumber = 0;

	// Actor 破壊タイプか、ジャンプアップタイプか, クリア判定か
	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	bool bIsActorDestroy = false;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	bool bIsJumpUp = false;

	UPROPERTY(EditAnywhere, Category = "Dialogue", meta = (MultiLine = "true"))
	bool bIsClear = false;

	void OnDialogueFinished();

};
