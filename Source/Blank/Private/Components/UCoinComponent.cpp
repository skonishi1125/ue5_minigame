#include "Components/UCoinComponent.h"

UUCoinComponent::UUCoinComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UUCoinComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UUCoinComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

