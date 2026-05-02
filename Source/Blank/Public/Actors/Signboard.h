#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Signboard.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class BLANK_API ASignboard : public AActor
{
	GENERATED_BODY()

public:
	ASignboard();
	virtual void Tick(float DeltaTime) override;
protected:
	virtual void BeginPlay() override;
private:
	// C++ からは継承不可だが、UE上からはアクセスできる
	// RO というのは、BPでメッシュを割り当てることは可能だが、ノードなどで置き換える処理に制限をかけている
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> SignboardMesh;
};
