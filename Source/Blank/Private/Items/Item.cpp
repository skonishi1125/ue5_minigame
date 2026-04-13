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
}

// Root に Scene(空コンポーネント)を付与して、子要素にMeshを付与する
void AItem::SetupComponents()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetupAttachment(RootScene);
}

