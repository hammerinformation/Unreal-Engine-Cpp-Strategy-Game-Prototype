#include "SoldierAnimInstance.h"

#include "Soldier.h"


void USoldierAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	Soldier = Cast<ASoldier>(TryGetPawnOwner());
}

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (Soldier)
	{
		Speed = Soldier->GetVelocity().Size();
	}
}
