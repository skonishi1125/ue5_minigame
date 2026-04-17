#include "Characters/BlankCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ABlankCharacter::ABlankCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	//SpringArm->bUsePawnControlRotation = true; // Controler の回転を SpringArmに反映させる

	bUseControllerRotationYaw = true;


}

void ABlankCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem
			= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer())
			)
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

}

void ABlankCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlankCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABlankCharacter::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABlankCharacter::Look);
		}

	}

}

void ABlankCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FVector ForwardDir = GetActorForwardVector();
		const FVector RightDir = GetActorRightVector();

		float MoveSpeed = 10.f;

		AddActorWorldOffset(ForwardDir * MoveVector.Y * MoveSpeed, true);
		AddActorWorldOffset(RightDir * MoveVector.X * MoveSpeed, true);

	}

}

void ABlankCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Controller (描写されない)に開店を加える
		// Controller に追従するように傾く設定フラグなどがあるので、追従させたい場合は別途 true としておく
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}
