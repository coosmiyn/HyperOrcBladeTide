// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine.h"
#include "Goblin.h"
#include "Components/SphereComponent.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a sphere component for the collision and set its radius
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCom"));
	CollisionComp->InitSphereRadius(10.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.0f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	RootComponent = CollisionComp;

	// Create the movement component for the projectile
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;

	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->ProjectileGravityScale = 0;

	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;


}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HIT"));

	AGoblin* Enemy = Cast<AGoblin>(OtherActor);

	if (Enemy)
	{
		FPointDamageEvent DmgEvent;
		DmgEvent.DamageTypeClass = PunchDamageType;
		DmgEvent.Damage = 25.0f;

		Enemy->TakeDamage(DmgEvent.Damage, DmgEvent, NULL, this);
	}

	Destroy();
}

