

#include "Soldier.h"

#include "PaperSpriteComponent.h"

ASoldier::ASoldier()
{
	PaperSprite=CreateDefaultSubobject<UPaperSpriteComponent>("PaperSprite");
	PaperSprite->SetupAttachment(GetMesh());

}

void ASoldier::BeginPlay()
{
	Super::BeginPlay();
	PaperSprite->SetHiddenInGame(false);
	PaperSprite->SetVisibility(false);

}


