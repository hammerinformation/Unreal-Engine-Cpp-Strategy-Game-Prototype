#include "MyPawn.h"
#include <algorithm>
#include <vector>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"
#include "Components/CanvasPanelSlot.h"
#include "Engine.h"
#include "Soldier.h"
#include "SoldierAIController.h"


AMyHUD* Hud{};
UImage* MyImage{};
UCanvasPanelSlot* ImageSlot{};
std::vector<ASoldier*> Soldiers;

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	Trigger->SetupAttachment(GetRootComponent());
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	Hud = Cast<AMyHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (Hud)
	{
		MyImage = Hud->MyUserWidget->MyImage;
		ImageSlot = Cast<UCanvasPanelSlot>(Hud->MyUserWidget->MyImage->Slot);
		MyImage->SetColorAndOpacity(FLinearColor(FVector4(0, 0, 0, .4f)));
	}

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyPawn::OnComponentBeginOverlap);
	Soldiers.reserve(50);
}


void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Magenta,FString::Printf(TEXT("Soldiers Size: %d"), (int)Soldiers.size()));

	if (bMousePressed)
	{
		LastPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		ImageSlot->SetSize(LastPos - FirstPos);
	}
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AMyPawn::MousePressed);
	PlayerInputComponent->BindKey(EKeys::LeftMouseButton, IE_Released, this, &AMyPawn::MouseReleased);
	PlayerInputComponent->BindKey(EKeys::RightMouseButton, IE_Pressed, this, &AMyPawn::RightMousePressed);
}

void AMyPawn::MousePressed()
{
	for (int i = 0; i < Soldiers.size(); ++i)
	{
		if (Soldiers[i] && Soldiers[i]->PaperSprite)
			Soldiers[i]->PaperSprite->SetVisibility(false);
	}
	if (Hud)
	{
		bMousePressed = true;

		if (APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
		{
			PlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
		}


		Soldiers.clear();

		Trigger->SetWorldLocation(FVector(0, 0, -99999));
		Trigger->SetBoxExtent(FVector::ZeroVector);
		FirstPos = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetWorld());
		ImageSlot->SetPosition(FirstPos);

		FHitResult HitResult;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(
			ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

		if (HitResult.bBlockingHit)
		{
			Point1 = HitResult.Location;
			FVector P = Point1;
			P.Z = 0;
			Point1 = P;
		}
	}
}


void AMyPawn::MouseReleased()
{
	if (Hud)
	{
		bMousePressed = false;

		FHitResult HitResult;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(
			ETraceTypeQuery::TraceTypeQuery1, false, HitResult);

		if (HitResult.bBlockingHit)
		{
			Point4 = HitResult.Location;
			FVector P = Point4;
			P.Z = 0;
			Point4 = P;
		}
		Point2 = FVector(Point1.X, Point4.Y, 0);
		Point3 = FVector(Point4.X, Point1.Y, 0);


		ImageSlot->SetPosition(FVector2D::ZeroVector);
		ImageSlot->SetSize(FVector2D::ZeroVector);

		/*
		DrawDebugBox(GetWorld(), (Point1 + Point2 + Point3 + Point4) / 4.f,
		             FVector(FMath::Abs(Point1.X - Point3.X), FMath::Abs(Point3.Y - Point4.Y), 100) / 2.f,
		             FColor::Emerald, false, 3);
*/

		Trigger->SetWorldLocation((Point1 + Point2 + Point3 + Point4) / 4.f);
		Trigger->SetBoxExtent(FVector(FMath::Abs(Point1.X - Point3.X), FMath::Abs(Point3.Y - Point4.Y), 500) / 2.f);

		if (!Soldiers.empty())
		{
			if (APlayerController* const PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
			{
				if (Soldiers.empty())
				{
					PlayerController->CurrentMouseCursor = EMouseCursor::Crosshairs;
					return;
				}

				PlayerController->CurrentMouseCursor = EMouseCursor::Hand;
			}
		}
	}
}

void AMyPawn::RightMousePressed()
{
	if (Soldiers.empty()) { return; }

	Trigger->SetWorldLocation(FVector(0, 0, -99999));
	Trigger->SetBoxExtent(FVector::ZeroVector);
	FHitResult Hit;
	GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorByChannel(
		ETraceTypeQuery::TraceTypeQuery1, false, Hit);

	const int N = Soldiers.size();
	const FVector Center = Hit.Location;
	const float Radius = N * 20;

	std::random_shuffle(Soldiers.begin(), Soldiers.end());

	for (int i = 0; i < N; i++)
	{
		const float Angle = i * 2 * PI / N;
		const float X = Center.X + Radius * FMath::Cos(Angle);
		const float Y = Center.Y + Radius * FMath::Sin(Angle);
		const FVector Location = FVector(X, Y, Soldiers[i]->GetActorLocation().Z);
		ASoldierAIController* const SoldierAIController = Cast<ASoldierAIController>(Soldiers[i]->GetController());
		SoldierAIController->MoveToLocation(Location);
	}
}

void AMyPawn::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	if (ASoldier* const Soldier = Cast<ASoldier>(OtherActor))
	{
		Soldiers.emplace_back(Soldier);
		Soldier->PaperSprite->SetVisibility(true);
	}
}
