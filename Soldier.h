
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PaperSpriteComponent.h"
#include "Soldier.generated.h"
UCLASS()
class STRATEGY_API ASoldier : public ACharacter
{
	GENERATED_BODY()

public:
	ASoldier();
protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere)
	UPaperSpriteComponent* PaperSprite; 

};
