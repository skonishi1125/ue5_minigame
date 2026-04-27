#include "GameMode/BlankGameMode.h"
#include "Logging/StructuredLog.h"

void ABlankGameMode::CheckWinCondition(int32 CurrentCoinCount)
{
	UE_LOGFMT(LogTemp, Log, "CheckWinCondition: Current {0} / Target {1}", CurrentCoinCount, TargetCoinCount);

	if (CurrentCoinCount >= TargetCoinCount)
	{
		UE_LOGFMT(LogTemp, Warning, "Game Cleared!");
		OnGameCleared();
	}
}

