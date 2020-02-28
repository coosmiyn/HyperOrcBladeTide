// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Pawn.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Crystal.generated.h"

UCLASS()
class HYPER_ORC_BLADETIDE_API ACrystal : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACrystal();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		UCapsuleComponent* CapsuleCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "CrystalCondition")
		float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "CrystalCondition")
		float GetHealth() const;

	virtual void PlayHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

	void ReplicateHit(float DamageTaken, struct FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser, bool bKilled);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
