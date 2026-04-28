#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TitleGameMode.generated.h"

UCLASS()
class BLANK_API ATitleGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<class UTitleWidget> TitleWidgetClass;
public:
	virtual void BeginPlay() override;

};
