#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlankCharacter.generated.h"

// カメラ
class USpringArmComponent;
class UCameraComponent;

// 入力
class UInputMappingContext;
class UInputAction;

// Groom 関連
class UGroomComponent;

// インタラクト
class UWidgetComponent;
class USphereComponent;

// 自作Componentなど
class UCoinComponent;

UCLASS()
class BLANK_API ABlankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlankCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetCoinCount() const;

protected:
	virtual void BeginPlay() override;
	virtual void UnPossessed() override;


	// ========= 入力関連 ===========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* CharacterMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;

	// ========= アクション関連 ===========
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ExecInteractive();
	//void PossessPlayerController();
	//void PossessPlayerControllerWithLine();
private:
	UFUNCTION()
	void OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Hair;

	UPROPERTY(VisibleAnywhere, Category = Hair)
	UGroomComponent* Eyebrows;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractWidget;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractArea;

	// 取得コイン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	int32 CoinCount = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UCoinComponent* CoinComponent;

};
