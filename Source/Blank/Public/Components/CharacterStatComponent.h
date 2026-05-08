// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CharacterStatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLANK_API UCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()
private:
	void UpdateMovementComponent();

	UPROPERTY(EditAnywhere)
	float BaseJumpZVelocity = 600.f;

	float CurrentJumpMultiplier = 1.0f;

protected:
	virtual void BeginPlay() override;
public:	
	UCharacterStatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void AddJumpMultiplier(float MultiplierToAdd);

	UFUNCTION(BlueprintCallable)
	void RemoveJumpMultiplier(float MultiplierToRemove);

};
