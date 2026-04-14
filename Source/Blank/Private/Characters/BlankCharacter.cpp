#include "Characters/BlankCharacter.h"

ABlankCharacter::ABlankCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ABlankCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void ABlankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

