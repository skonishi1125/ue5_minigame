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
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"

ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetCapsuleHalfHeight(40.f);
	Capsule->SetCapsuleRadius(40.f);
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Ray をキャッチできるようにする
	Capsule->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block); // Landscape の地面を貫通させない
	SetRootComponent(Capsule);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	BirdMesh->SetupAttachment(GetRootComponent()); // = Capsule の子にする
	BirdMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

	// Movement
	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->MaxSpeed = 1500.f;
	FloatingPawnMovement->Acceleration = 5000.f; // 入力開始時の加速パラメータ
	FloatingPawnMovement->Deceleration = 5000.f; // 入力をやめたときの減速パラメータ

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

	// UI系統
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen); // カメラに向くようにする
	InteractWidget->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	InteractWidget->SetVisibility(false);

	// Interact関連
	InteractArea = CreateDefaultSubobject<USphereComponent>(TEXT("IntaractArea"));
	InteractArea->SetSphereRadius(60.f);
	InteractArea->SetupAttachment(GetRootComponent());
	InteractArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 物理衝突は考慮しないが処理は動かす
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore); // すべて無視
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn 系だけすり抜け検知する

}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (InteractArea)
	{
		InteractArea->OnComponentBeginOverlap.AddDynamic(this, &ABird::OnInteractAreaBeginOverlap);
		InteractArea->OnComponentEndOverlap.AddDynamic(this, &ABird::OnInteractAreaEndOverlap);
	}

}

void ABird::OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABlankCharacter* Character = Cast<ABlankCharacter>(OtherActor))
		{
			if (InteractWidget)
			{
				InteractWidget->SetVisibility(true);
			}
		}
	}
}

void ABird::OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABlankCharacter* Character = Cast<ABlankCharacter>(OtherActor))
		{
			if (InteractWidget)
			{
				InteractWidget->SetVisibility(false);
			}
		}
	}
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

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

		//AddActorWorldOffset(ForwardDirection * MovementVector.Y * MoveSpeed, true);
		//AddActorWorldOffset(RightDirection * MovementVector.X * MoveSpeed, true);
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


