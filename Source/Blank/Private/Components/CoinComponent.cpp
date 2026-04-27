
#include "Components/CoinComponent.h"
#include "GameMode/BlankGameMode.h"
#include "Kismet/GameplayStatics.h"


UCoinComponent::UCoinComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCoinComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCoinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCoinComponent::AddCoin()
{
	CoinCount++;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::Printf(TEXT("UCoinComponent, Coin Count: %d"), CoinCount));
	}

	OnCoinCountChanged.Broadcast(CoinCount);

	if (ABlankGameMode* GameMode = Cast<ABlankGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->CheckWinCondition(CoinCount);
	}

}

int32 UCoinComponent::GetCoinCount() const
{
	return CoinCount;
}