#include "GameMode/BlankGameMode.h"
#include "Logging/StructuredLog.h"
#include "Characters/BlankCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CoinComponent.h"

void ABlankGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (ABlankCharacter* BlankCharacter = Cast<ABlankCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0))) {
		if (UCoinComponent* CoinComponent = BlankCharacter->FindComponentByClass<UCoinComponent>())
		{
			CoinComponent->OnCoinCountChanged.AddDynamic(this, &ABlankGameMode::HandleCoinCountChanged);
			UE_LOGFMT(LogTemp, Warning, "BlankGameMode: successfully bound to CoinComponent");
		}

	}
}

// デリゲート用 Broadcast時に自動で呼ばれる関数
void ABlankGameMode::HandleCoinCountChanged(int32 NewCoinCount)
{
	CheckWinCondition(NewCoinCount);
}

void ABlankGameMode::CheckWinCondition(int32 CurrentCoinCount)
{
	UE_LOGFMT(LogTemp, Warning, "CheckWinCondition: Current {0} / Target {1}", CurrentCoinCount, TargetCoinCount);

	if (CurrentCoinCount >= TargetCoinCount)
	{
		UE_LOGFMT(LogTemp, Warning, "Game Cleared!");
		OnGameCleared();
	}
}


