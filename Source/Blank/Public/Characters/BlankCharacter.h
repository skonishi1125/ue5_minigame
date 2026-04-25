#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BlankCharacter.generated.h"

// クラスの定義を展開するマクロ
// コイン枚数変化を示すデリゲートの宣言
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinCountChanged, int32, NewCoinCount);

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

UCLASS()
class BLANK_API ABlankCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABlankCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// コイン取得時に外部から呼ぶため、public とする
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void AddCoin();

	UFUNCTION(BlueprintCallable, Category = "Stats")
	int32 GetCoinCount() const;

	// マクロで定義したクラスを使用し、
	// Blueprint 側からイベントを受け取るための関数
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCoinCountChanged OnCoinCountChanged;

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
	void PossessPlayerController();
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

};
