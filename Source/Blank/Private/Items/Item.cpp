#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Characters/BlankCharacter.h"

float AItem::TransformedSin()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);

}

AItem::AItem()
{
	// Tick()の有効化
	PrimaryActorTick.bCanEverTick = true;
	SetupComponents();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	//UE_LOG(LogTemp, Warning, TEXT("Begin Play Call C++!"));

	if (InteractArea)
	{
		InteractArea->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnInteractAreaBeginOverlap);
	}

}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	// 上下ホバリング
	AddActorWorldOffset(FVector(0, 0, TransformedSin()));

	// 左回転処理
	RotateObject(DeltaTime);
}

void AItem::RotateObject(float DeltaTime)
{
	// [deg/s] * [s] = [deg] : 1フレーム(delta)あたり 何度回転するのか
	const float DeltaRotation = RotationSpeed * DeltaTime;
	AddActorLocalRotation(FRotator(.0f, DeltaRotation, .0f)); // Tick でフレームごとに回転処理をする
}

// Root に Scene(空コンポーネント)を付与して、子要素にMeshを付与する
void AItem::SetupComponents()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetupAttachment(RootScene);

	InteractArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractArea"));
	InteractArea->SetSphereRadius(60.f);
	InteractArea->SetupAttachment(RootScene); // SetRootComponent() でもいいと思う
	InteractArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InteractArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AItem::OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABlankCharacter* Character = Cast<ABlankCharacter>(OtherActor))
		{
			UE_LOG(LogTemp, Warning, TEXT("Touched Player"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Another touched coin"));
		}
	}
}

