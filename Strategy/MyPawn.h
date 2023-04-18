
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"


UCLASS()
class STRATEGY_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


private:
	void MousePressed();
	void MouseReleased();
	
	void RightMousePressed();
	FVector2D FirstPos{};
	FVector2D LastPos{};


	FVector Point1{};
	FVector Point2{};
	FVector Point3{};
	FVector Point4{};
	bool bMousePressed{};
	bool bRightMousePressed{};

	UPROPERTY(EditAnywhere)
	class UBoxComponent* Trigger;
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);
};
