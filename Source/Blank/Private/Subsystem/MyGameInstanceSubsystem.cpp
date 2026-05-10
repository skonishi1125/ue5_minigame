#include "Subsystem/MyGameInstanceSubsystem.h"
#include "GameFramework/GameUserSettings.h"

void UMyGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    ApplyLowScalability();

    UE_LOG(LogTemp, Warning, TEXT("UMyGameInstanceSubsystem"));

}

void UMyGameInstanceSubsystem::ApplyLowScalability()
{
    UGameUserSettings* UserSettings = GEngine->GetGameUserSettings();

    if (UserSettings)
    {
        // 0: Low, 1: Medium, 2: High, 3: Epic, 4: Cinematic
        UserSettings->SetOverallScalabilityLevel(0);

        // 変更を適用し、設定ファイル(GameUserSettings.ini)に保存
        UserSettings->ApplySettings(true);
    }
}
