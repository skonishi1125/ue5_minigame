#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BLANK_API AItem : public AActor
{
	GENERATED_BODY()
private:
	void AttachMeshComponent();
protected:
	virtual void BeginPlay() override;
	// ルートとなる基準点（座標と回転のデータのみを持つ）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootScene;

	// UPROPERTY: UE5のGabage Collection 対象下とする
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;
public:
	AItem();
	virtual void Tick(float DeltaTime) override;
};
