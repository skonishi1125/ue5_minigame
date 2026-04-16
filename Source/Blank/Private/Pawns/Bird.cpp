#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(20.f);
	Capsule->SetCapsuleRadius(15.f);
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	BirdMesh->SetupAttachment(GetRootComponent()); // = Capsule の子にする
	BirdMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// カメラ系コンポーネント
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmBird"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;
	//SpringArm->AddLocalRotation(FRotator(-20.f, 0.f, 0.f));
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	// Mapping Context の登録
	// GetController()でPawnで使われているコントローラを持ってくる
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

}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (MoveAction)
		{
			// ETriggerEvent::Triggered は入力が続いている間、毎フレーム呼ばれる処理
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABird::Move);
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

		// 移動速度の倍率（必要に応じて調整）
		float MoveSpeed = 10.0f;

		AddActorLocalOffset(ForwardDirection * MovementVector.Y * MoveSpeed, true);
		AddActorLocalOffset(RightDirection * MovementVector.X * MoveSpeed, true);
	}
}

