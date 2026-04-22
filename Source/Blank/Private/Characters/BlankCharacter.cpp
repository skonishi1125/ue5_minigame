#include "Characters/BlankCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GroomComponent.h"
#include "DrawDebugHelpers.h"
#include "Pawns/Bird.h"
#include "Engine/HitResult.h"
#include "CollisionShape.h"
#include "CollisionQueryParams.h"
#include "Engine/OverlapResult.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include "Logging/StructuredLog.h"

ABlankCharacter::ABlankCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Controller を所持していない場合でも物理演算を有効化する
	GetCharacterMovement()->bRunPhysicsWithNoController = true;

	// Rayを検知できるようにする
	UCapsuleComponent* Capsule = GetCapsuleComponent(); // ACharacter デフォルト付与のものを使う
	Capsule->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// 進行方向に振り向く設定にするかどうかと、振り向き速度
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// Controller の回転に Character 自身は依存しないことを明示する
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmBlankCharacter"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 200.f;
	SpringArm->bUsePawnControlRotation = true; // Controler の回転を SpringArmに反映させる
	SpringArm->SetRelativeRotation(FRotator(-20.f, 0.f, 0.f));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	// Auto Possess Player の設定。 不要かも
	//AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Grooms 関連
	Hair = CreateDefaultSubobject<UGroomComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh()); // APlayer がデフォルトで持つ CharacterMesh の子に指定
	Hair->AttachmentName = FString("head"); // Socket を使ってくっつける
	Eyebrows = CreateDefaultSubobject<UGroomComponent>(TEXT("Eyebrows"));
	Eyebrows->SetupAttachment(GetMesh());
	Eyebrows->AttachmentName = FString("head");

	// UI 関連
	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen);
	InteractWidget->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	InteractWidget->SetVisibility(false);

	// Interact 関連
	InteractArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractArea"));
	InteractArea->SetSphereRadius(60.f);
	InteractArea->SetupAttachment(GetRootComponent());
	InteractArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void ABlankCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (InteractArea)
	{
		InteractArea->OnComponentBeginOverlap.AddDynamic(this, &ABlankCharacter::OnInteractAreaBeginOverlap);
		InteractArea->OnComponentEndOverlap.AddDynamic(this, &ABlankCharacter::OnInteractAreaEndOverlap);
	}

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

		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		if (InteractAction)
		{
			EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ABlankCharacter::PossessPlayerController);
		}

	}

}

void ABlankCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MoveVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{

		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// 回転行列
		const FVector ForwardDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // UEでいう正面
		const FVector RightDir = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // UEでいう真右

		// ACharacter の持つ移動用関数
		AddMovementInput(ForwardDir, MoveVector.Y);
		AddMovementInput(RightDir, MoveVector.X);

		// こちらでも動くが、bOrientRotationToMovement など、移動方向に応じて振り向く処理などが適用されない
		//AddActorWorldOffset(ForwardDir * MoveVector.Y * MoveSpeed, true);
		//AddActorWorldOffset(RightDir * MoveVector.X * MoveSpeed, true);

		// 単純に Actor の向いている方向に進めたい場合はこちら
		//const FVector ForwardDir = GetActorForwardVector();
		//const FVector RightDir = GetActorRightVector();
	}

}

void ABlankCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Controller (画面には描写されないもの)に回転を加える
		// Controller に追従するように傾く設定フラグなどがあるので、今回これに追従させたいので true としておくこと
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}

}


void ABlankCharacter::PossessPlayerController()
{
	//UE_LOG(LogTemp, Warning, TEXT("Shape: Possess Pressed"));

	FVector Start = GetActorLocation();
	FVector End = Start + (GetActorForwardVector() * 10.f); // 始点と終点を中心から少しだけずらす

	FCollisionShape SphereShape = FCollisionShape::MakeSphere(300.f);

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// SweepSingleByChannel だと、円形に取得すると地面を感知してしまう
	bool bHit = GetWorld()->OverlapMultiByChannel(
		OverlapResults,
		Start,
		FQuat::Identity, // 無回転
		ECC_Visibility,
		SphereShape,     // 作成した球体を渡す
		CollisionParams
	);

	// デバッグ 円形 緑 32はどの程度分割して球体を表すか。
	DrawDebugSphere(GetWorld(), Start, SphereShape.GetSphereRadius(), 32, bHit ? FColor::Green : FColor::Red, false, 2.f);

	if (bHit)
	{
		// structに対してループを回す場合は、参照として渡す
		// &がないとループのたびにデータのコピーが発生してパフォーマンスが落ちる
		for (const FOverlapResult& Hit : OverlapResults)
		{
			// UEではActorなどのObjectはポインタで扱うというルールがあるので、それに従う
			// https://dev.epicgames.com/documentation/unreal-engine/unreal-object-handling-in-unreal-engine?lang=ja (参照の自動更新)
			AActor* HitActor = Hit.GetActor();

			// Cast()はnullptrが渡されたときでもクラッシュしない設計となっているので、HitActorをnullptrチェックしなくてよい
			if (ABird* HitBird = Cast<ABird>(HitActor))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Bird Found in Sphere!"));

				if (APlayerController* PC = Cast<APlayerController>(GetController()))
				{
					PC->Possess(HitBird);
				}

				return;
			}
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("No Bird nearby"));
}

// Controller が外れた時に Animation が流しっぱなしとなる挙動を防ぐ
void ABlankCharacter::UnPossessed()
{
	Super::UnPossessed();

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (MovementComponent != nullptr)
	{
		MovementComponent->StopMovementImmediately();
	}

	StopJumping();

}

void ABlankCharacter::OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOGFMT(LogTemp, Verbose, "Start OnInteractAreaBeginOverlap");

	if (OtherActor && OtherActor != this)
	{
		UE_LOGFMT(LogTemp, Verbose, "Capture OnInteractAreaBeginOverlap");

		if (ABird* Bird = Cast<ABird>(OtherActor))
		{
			UE_LOGFMT(LogTemp, Verbose, "Bird OnInteractAreaBeginOverlap");

			if (InteractWidget)
			{
				InteractWidget->SetVisibility(true);
			}
		}
	}
}

void ABlankCharacter::OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABird* Bird = Cast<ABird>(OtherActor))
		{
			if (InteractWidget)
			{
				InteractWidget->SetVisibility(false);
			}
		}
	}
}








/*
	未使用だが、カメラからRayを使ってCollisionを検知するコードの例

void ABlankCharacter::PossessPlayerControllerWithLine()
{
	UE_LOG(LogTemp, Warning, TEXT("Possess Pressed!"));

	// 始点と終点を決定して、Rayの長さを定義する
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + (Camera->GetForwardVector() * 1000.f);

	// Line Trace準備
	FHitResult HitResult; // 当たった対象、座標、表面角度などを格納するための構造体
	FCollisionQueryParams CollisionParams; // Trace の挙動を設定するための構造体
	CollisionParams.AddIgnoredActor(this); // BlankCharacter自身は干渉しないようにする

	// GetWorld()->LineTraceSingleByChannel() で Rayを呼び出す
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult, // 当たったとき、格納するもの
		Start,
		End,
		ECC_Visibility, // チャンネル
		CollisionParams // Trace 挙動の調整用データ
	);

	// デバッグ用にRayを可視化する
	DrawDebugLine(GetWorld(), Start, End, bHit ? FColor::Green : FColor::Red, false, 2.f, 0, 2.f);

	if (bHit)
	{
		// 当たった Actor が ABird かどうかを Cast して確認
		AActor* HitActor = HitResult.GetActor();

		if (ABird* HitBird = Cast<ABird>(HitActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Bird Found!"));

			// 現在の PlayerController を取得し、鳥へ Possess（憑依）する
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				PC->Possess(HitBird);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Bird"));
	}

}
*/
