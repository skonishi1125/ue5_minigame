// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/BlankCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/BlankAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

void UBlankAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	BlankCharacter = Cast<ABlankCharacter>(TryGetPawnOwner());
	if (BlankCharacter != nullptr)
	{
		BlankCharacterMovement = BlankCharacter->GetCharacterMovement();
	}

}

void UBlankAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (BlankCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(BlankCharacterMovement->Velocity);
		IsFalling = BlankCharacterMovement->IsFalling();

		if (GroundSpeed != 0)
		{
			UE_LOG(LogTemp, Log, TEXT("The vector value is: %s"), *BlankCharacterMovement->Velocity.ToString());
			UE_LOG(LogTemp, Warning, TEXT("The boolean value is %s"), (IsFalling ? TEXT("true") : TEXT("false")));
		}
	}

}