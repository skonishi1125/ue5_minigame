
#include "Components/CharacterStatComponent.h"
#include "Characters/BlankCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

UCharacterStatComponent::UCharacterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCharacterStatComponent::UpdateMovementComponent()
{
	// 所有者（Character）を取得し、計算した最終的な値を CharacterMovement に適用する
	if (ABlankCharacter* OwnerCharacter = Cast<ABlankCharacter>(GetOwner()))
	{
		if (UCharacterMovementComponent* Movement = OwnerCharacter->GetCharacterMovement())
		{
			Movement->JumpZVelocity = BaseJumpZVelocity * CurrentJumpMultiplier;
		}
	}
}

void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCharacterStatComponent::AddJumpMultiplier(float MultiplierToAdd)
{
	CurrentJumpMultiplier += MultiplierToAdd;
	UpdateMovementComponent();
}

void UCharacterStatComponent::RemoveJumpMultiplier(float MultiplierToRemove)
{
	CurrentJumpMultiplier -= MultiplierToRemove;
	UpdateMovementComponent();
}

