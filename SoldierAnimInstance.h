#pragma once
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoldierAnimInstance.generated.h"


UCLASS()
class STRATEGY_API USoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	class ASoldier* Soldier{};
public:
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;
	UPROPERTY(BlueprintReadOnly)
	float Speed;
	
};
