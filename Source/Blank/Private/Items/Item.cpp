#include "Items/Item.h"

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
	UE_LOG(LogTemp, Warning, TEXT("Begin Play Call C++!"));
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
}

