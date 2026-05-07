#include "Actors/Signboard.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "Characters/BlankCharacter.h"
#include "Controller/BlankPlayerController.h"
#include "Components/CoinComponent.h"

ASignboard::ASignboard()
{
	PrimaryActorTick.bCanEverTick = false;

	// 初期設定
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);

	SignboardMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SignboardMeshComponent"));
	SignboardMesh->SetupAttachment(RootScene);

	InteractArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractArea"));
	InteractArea->SetSphereRadius(60.f);
	InteractArea->SetupAttachment(RootScene);

	MessageTexts.Add(INVTEXT("1ページ目のテキストです。\n改行は Shift + Enter でできます。\n1ページ目のテキスト。1ページ目のテキスト。"));

	InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
	InteractWidget->SetupAttachment(GetRootComponent());
	InteractWidget->SetWidgetSpace(EWidgetSpace::Screen); // ビルボードのようにする
	InteractWidget->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
	InteractWidget->SetVisibility(false);

}

void ASignboard::BeginPlay()
{
	Super::BeginPlay();

	if (InteractArea)
	{
		InteractArea->OnComponentBeginOverlap.AddDynamic(this, &ASignboard::OnInteractAreaBeginOverlap);
		InteractArea->OnComponentEndOverlap.AddDynamic(this, &ASignboard::OnInteractAreaEndOverlap);
	}
}

void ASignboard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASignboard::Interact(APawn* Interactor)
{
	//UE_LOG(LogTemp, Warning, TEXT("看板を読みました！テキスト: %s"), *MessageText.ToString());
	UE_LOG(LogTemp, Warning, TEXT("ASignboard::Interact()"));


	if (ABlankPlayerController* PC = Cast<ABlankPlayerController>(Interactor->GetController()))
	{

		TArray<FText> SelectedTexts = MessageTexts;

		for (const FConditionalDialogue& Item : ConditionalDialogues)
		{
			// コイン取得処理
			if (Item.ConditionTag == "FiveCoins")
			{
				if (UCoinComponent* CoinComp = Interactor->FindComponentByClass<UCoinComponent>())
				{
					if (CoinComp->GetCoinCount() >= 5)
					{
						SelectedTexts = Item.DialogueTexts;
						if (TargetActorToDestroy)
						{
							TargetActorToDestroy->Destroy();
							TargetActorToDestroy = nullptr;
						}

						CoinComp->UseCoin(5);
					}
				}
			}
		}

		PC->ShowDialogue(SelectedTexts);

		if (InteractWidget)
		{
			InteractWidget->SetVisibility(false);
		}

	}
}

void ASignboard::OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABlankCharacter* Character = Cast<ABlankCharacter>(OtherActor))
		{
			if (InteractWidget)
			{
				InteractWidget->SetVisibility(true);
			}
		}
	}

}

void ASignboard::OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor != this)
	{
		if (ABlankCharacter* Character = Cast<ABlankCharacter>(OtherActor))
		{
			if (InteractWidget)
			{
				InteractWidget->SetVisibility(false);
			}
		}
	}
}

