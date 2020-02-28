// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "Components/CapsuleComponent.h"
#include "Engine.h"
#include "MainCharacter.h"
#include "Crystal.h"
#include "GameFramework/Actor.h"

// Sets default values
AGoblin::AGoblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

	// Set the capsule component collision responses
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCapsuleHalfHeight(82.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);

	// Create the melee collision component
	MeleeCollisionComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("MeleeCollision"));

	// Set it's position in front of the Goblin, relative to the root component
	MeleeCollisionComp->SetRelativeLocation(FVector(45.0f, 0.0f, 0.0f));
	MeleeCollisionComp->SetCapsuleHalfHeight(82.0f);
	MeleeCollisionComp->SetCapsuleRadius(42.0f, false);

	MeleeCollisionComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeleeCollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	MeleeCollisionComp->SetupAttachment(GetCapsuleComponent());
	MeleeCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGoblin::OnMeleeCompBeginOverlap);

	// Setup the stats
	Health = 100;
	MeleeDamage = 24.0f;
	MeleeStrikeCooldown = 1.0f;
}

// Called when the game starts or when spawned
void AGoblin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGoblin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGoblin::OnMeleeCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Blue, TEXT("Overlap"));
	TimerHandle_MeleeAttack.Invalidate();

	if (Cast<ACrystal>(OtherActor))
	{
		PerformMeleeStrike(OtherActor);

		/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
		GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &AGoblin::OnRetriggerMeleeStrike, MeleeStrikeCooldown, true);
	}

}

void AGoblin::OnRetriggerMeleeStrike()
{
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;

	// Check if any actors are in the attack range
	MeleeCollisionComp->GetOverlappingActors(Overlaps, ACrystal::StaticClass());
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ACrystal* OverlappingPawn = Cast<ACrystal>(Overlaps[i]);
		if (OverlappingPawn)
		{
			PerformMeleeStrike(OverlappingPawn);
			//break; /* Uncomment to only attack one pawn maximum */
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
}

void AGoblin::PerformMeleeStrike(AActor* HitActor)
{
	// Check if the actor is a Crystal. If so, attack
	// Add more actors the Goblin can attack, the player for example, or traps.
	ACrystal* OtherPawn = Cast<ACrystal>(HitActor);
	if (OtherPawn)
	{
		GEngine->AddOnScreenDebugMessage(-2, 5.0f, FColor::Blue, TEXT("Attacking"));

		/* Set to prevent a zombie to attack multiple times in a very short time */
		LastMeleeAttackTime = GetWorld()->GetTimeSeconds();

		FPointDamageEvent DmgEvent;
		DmgEvent.DamageTypeClass = PunchDamageType;
		DmgEvent.Damage = MeleeDamage;

		HitActor->TakeDamage(DmgEvent.Damage, DmgEvent, GetController(), this);

		SimulateMeleeStrike();
	}
}

void AGoblin::SimulateMeleeStrike_Implementation()
{
}

void AGoblin::PlayHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{

}

// TO DO: Implement invulnerability if needed (ActualDamage is implemented for that)
float AGoblin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;

	if (Health <= 0)
	{
		Destroy();
	}
	return ActualDamage;
}

