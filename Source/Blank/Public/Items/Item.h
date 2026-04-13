#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class BLANK_API AItem : public AActor
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
public:
	AItem();
	virtual void Tick(float DeltaTime) override;
};
