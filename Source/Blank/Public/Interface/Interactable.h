#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};

class BLANK_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Eキーを押したときのインタラクト実行関数 引数として、誰がインタラクトしたかを受け取っておく
	// = 0 : 純粋仮想関数。このInterfaceを継承するクラスは必ずこの処理を実装するという制約を持たせる
	virtual void Interact(APawn* Interactor) = 0;
};
