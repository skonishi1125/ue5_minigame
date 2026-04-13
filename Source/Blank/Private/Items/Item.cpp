#include "Items/Item.h"

AItem::AItem()
{
	// Tick()の有効化
	PrimaryActorTick.bCanEverTick = true;
	AttachMeshComponent();
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin Play Call C++!"));
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Root に Scene(空コンポーネント)を付与して、子要素にMeshを付与する
void AItem::AttachMeshComponent()
{
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	ItemMesh->SetupAttachment(RootScene);
}

