// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "STypes.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Goblin.generated.h"

UCLASS()
class HYPER_ORC_BLADETIDE_API AGoblin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoblin();

	// AI components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class ABotWaypoint* NextWaypoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attacking")
		UCapsuleComponent* MeleeCollisionComp;

protected:


	/* A pawn is in melee range */
	UFUNCTION()
		void OnMeleeCompBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnRetriggerMeleeStrike();

	/* Deal damage to the Actor that was hit by the punch animation */
	UFUNCTION(BlueprintCallable, Category = "Attacking")
		void PerformMeleeStrike(AActor* HitActor);

	UFUNCTION(Reliable, NetMulticast)
		void SimulateMeleeStrike();

	void SimulateMeleeStrike_Implementation();

	// Damage types
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		TSubclassOf<UDamageType> PunchDamageType;

	// Stats
	UPROPERTY(EditDefaultsOnly, Category = "Attacking")
		float MeleeDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
		float Health;

	/* Last time we attacked something */
	float LastMeleeAttackTime;

	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	/* Minimum time between melee attacks */
	float MeleeStrikeCooldown;

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);

public:

	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
		bool bIsPunching;
};
