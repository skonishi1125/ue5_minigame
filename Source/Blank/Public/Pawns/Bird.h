#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Interface/Interactable.h"

#include "Bird.generated.h"

class UCapsuleComponent;
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;
class UWidgetComponent;
class USphereComponent;
class ABlankCharacter;

UCLASS()
class BLANK_API ABird : public APawn, public IInteractable
{
	GENERATED_BODY()
public:
	ABird();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetBlankCharacter(ABlankCharacter* Character);
	void Interact(APawn* Interactor) override;

protected:
	virtual void BeginPlay() override;


	// ========= 入力関連 ===========
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* BirdMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* InteractAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UpAction;


	// ========= アクション関連 ===========
	// ポインタで宣言する場合は前方宣言が使える
	// ただし参照として引数を取る場合は前方宣言ができない。この場合はもうincludeしてしまってよい
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	//void PossessPlayerController();
	void PossessPlayerControllerAnyWhere();
	void ExecInteractive();
	void Up(const FInputActionValue& Value);

private:
	UPROPERTY(VisibleAnywhere)
	FTransform DefaultSpawnTransform;

	UFUNCTION()
	void OnInteractAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnInteractAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	UFloatingPawnMovement* FloatingPawnMovement;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* InteractWidget;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractArea; // Character 感知用

	UPROPERTY(EditAnywhere, Category = "Possess")
	ABlankCharacter* BlankCharacter;


};
