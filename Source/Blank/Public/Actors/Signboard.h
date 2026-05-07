#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"

#include "Signboard.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

USTRUCT(BlueprintType)
struct FConditionalDialogue
{
	GENERATED_BODY()

	// 条件を判定するためのタグ "HasEnoughCoins", "OpenedSecretGate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Condition")
	FName ConditionTag;

	// 条件を満たしたときに表示するテキスト
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue", meta = (MultiLine = "true"))
	TArray<FText> DialogueTexts;
};

UCLASS()
class BLANK_API ASignboard : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ASignboard();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(APawn* Interactor) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MultiLine = "true"))
	TArray<FConditionalDialogue> ConditionalDialogues;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true", MultiLine = "true"))
	TArray<FText> MessageTexts;

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


};
