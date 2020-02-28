// Fill out your copyright notice in the Description page of Project Settings.


#include "Crystal.h"
#include "Goblin.h"
#include "Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"

// Sets default values
ACrystal::ACrystal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// main collision component
	CapsuleCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CapsuleCollisionComponent->SetupAttachment(RootComponent);

	// Set the collision response channels
	CapsuleCollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleCollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	RootComponent = CapsuleCollisionComponent;

	// Create the mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static mesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Set the health
	Health = 100.0f;

}

float ACrystal::GetMaxHealth() const
{
	return MaxHealth;
}

float ACrystal::GetHealth() const
{
	return Health;
}

// Called when the game starts or when spawned
void ACrystal::BeginPlay()
{
	Super::BeginPlay();
	
}

float ACrystal::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	Health -= ActualDamage;

	// Implement if you want to make the object unkillable for a period of time
	//if (Health <= 0)
	//{
	//	// TO DO: Implement death
	//}
	//else
	//{
	//	APawn* Pawn = EventInstigator ? EventInstigator->GetPawn() : nullptr;
	//	PlayHit(ActualDamage, DamageEvent, Pawn, DamageCauser, false);
	//}

	if (Health <= 0)
	{
		Destroy();
	}

	return ActualDamage;
}

void ACrystal::PlayHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
	ReplicateHit(DamageTaken, DamageEvent, PawnInstigator, DamageCauser, bKilled);
}

void ACrystal::ReplicateHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled)
{
}

// Called every frame
void ACrystal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

