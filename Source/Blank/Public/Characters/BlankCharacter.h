#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlankCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class BLANK_API ABlankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlankCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void BeginPlay() override;

	// ========= 入力関連 ===========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	// ========= アクション関連 ===========
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

};
