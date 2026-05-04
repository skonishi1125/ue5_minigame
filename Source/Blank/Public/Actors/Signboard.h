#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Interactable.h"

#include "Signboard.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;
class UWidgetComponent;

UCLASS()
class BLANK_API ASignboard : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	ASignboard();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact(APawn* Interactor) override;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FText MessageText;

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
