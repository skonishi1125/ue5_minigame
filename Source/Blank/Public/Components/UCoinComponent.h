#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UCoinComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLANK_API UUCoinComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUCoinComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
};
