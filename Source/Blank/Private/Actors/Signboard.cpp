#include "Actors/Signboard.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ASignboard::ASignboard()
{
	PrimaryActorTick.bCanEverTick = true;

	// 初期設定
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootComponent);

	SignboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignboardMeshComponent"));
	SignboardMesh->SetupAttachment(RootScene);

}

void ASignboard::BeginPlay()
{
	Super::BeginPlay();
}

void ASignboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

