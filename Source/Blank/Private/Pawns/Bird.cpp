#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Logging/StructuredLog.h"
#include "CollisionShape.h"
#include "Engine/OverlapResult.h"
#include "CollisionQueryParams.h"
#include "Characters/BlankCharacter.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(40.f);
	Capsule->SetCapsuleRadius(40.f);
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Ray をキャッチできるようにする
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	BirdMesh->SetupAttachment(GetRootComponent()); // = Capsule の子にする
	BirdMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// カメラ系コンポーネント
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmBird"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bUsePawnControlRotation = true; // Controler の回転を SpringArmに反映させる

	//SpringArm->AddLocalRotation(FRotator(-20.f, 0.f, 0.f));
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Controller の Yaw(左右回転)に、Bird 自身も追従させる
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;

}

void ABird::BeginPlay()
{
	Super::BeginPlay();
}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Player Controller が渡されたときに発火する
void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Mapping Context の登録
	// まずGetController()でPawnに紐づけられたControllerを取得。
	// それがPlayerのControllerだった時の処理（AIならAIのコントローラが返ってきてスキップするという感じ）
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (DefaultMappingContext)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
			}
		}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			// ETriggerEvent::Triggered は入力が続いている間、毎フレーム呼ばれる処理
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
		}

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABird::Look);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABird::PossessPlayerController);
		}
	}

}

void ABird::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FVector ForwardDirection = GetActorForwardVector();
		const FVector RightDirection = GetActorRightVector();

		// 移動速度倍率 一旦 10
		float MoveSpeed = 10.0f;

		AddActorWorldOffset(ForwardDirection * MovementVector.Y * MoveSpeed, true);
		AddActorWorldOffset(RightDirection * MovementVector.X * MoveSpeed, true);
	}
}

void ABird::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// ControllerのYaw(Z軸/左右)とPitch(Y軸/上下)にマウスの移動量を足す
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}

void ABird::PossessPlayerController()
{
	// LOGFMT を使ってみる
	UE_LOGFMT(LogTemp, Display, "Actor :{Name}", this->GetName());

	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 10.f);

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(300.f);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults, Start, FQuat::Identity, ECC_Visibility, SphereShape, CollisionParams
	);

	DrawDebugSphere(GetWorld(), Start, SphereShape.GetSphereRadius(), 32, bHit ? FColor::Green : FColor::Red, false, 2.f);

	if (bHit)
	{
		for (const FOverlapResult& Hit : OverlapResults)
		{
			AActor* HitActor = Hit.GetActor();

			if (ABlankCharacter* HitCharacter = Cast<ABlankCharacter>(HitActor))
			{
				UE_LOGFMT(LogTemp, Display, "Character found :{Name}", HitCharacter->GetName());
				if (APlayerController* PC = Cast<APlayerController>(GetController()))
				{
					PC->Possess(HitCharacter);
				}
				return;
			}

		}
	}
	UE_LOGFMT(LogTemp, Verbose, "Character not found.");



}


