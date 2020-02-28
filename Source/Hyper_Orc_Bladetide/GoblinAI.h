// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Goblin.h"
#include "BotWaypoint.h"
#include "MainCharacter.h"
#include "Crystal.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "GoblinAI.generated.h"

UCLASS()
class HYPER_ORC_BLADETIDE_API AGoblinAI : public AAIController
{
	GENERATED_BODY()

public:

	AGoblinAI();

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION()
		void OnPawnDetected(const TArray<AActor*>& DetectedPawns);

	UFUNCTION()
		void OnTargetDetected(AActor* Actor, FAIStimulus const stimulus);

public:

	// Crystal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
		ACrystal* Crystal;

	// AI Components
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		UBlackboardComponent* BlackboardComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		class UAISenseConfig_Sight* Sight;

	// Sight variables
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightRadius = 2000.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightAge = 0.1f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AISightLoseRadius = AISightRadius + 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
		float AIFieldOfView = 60.0f;

	// Blackboard keys
	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName WaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName HasLineOfSightKeyName;

	UPROPERTY(EditDefaultsOnly, Category = AI)
		FName CrystalKeyName;

	// Blackboard keys get/set methods
	bool GetLineOfSight();

	void SetLineOfSight(bool value);

	ABotWaypoint* GetWaypoint();
	
	void SetWaypoint(ABotWaypoint* NewWaypoint);

	AMainCharacter* GetTargetEnemy();

	void SetTargetEnemy(APawn* NewTarget);

	ACrystal* GetCrystal();

	void SetCrystal(AActor* Target);

	FORCEINLINE UBehaviorTreeComponent* GetBehaviourComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
