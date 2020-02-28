// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAI.h"
#include "UObject/UObjectGlobals.h"
#include "Engine.h"

AGoblinAI::AGoblinAI()
{
	// Create the sight component
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	// Setup the sight parameters
	Sight->SightRadius = AISightRadius;
	Sight->LoseSightRadius = AISightLoseRadius;
	Sight->PeripheralVisionAngleDegrees = AIFieldOfView;
	Sight->SetMaxAge(AISightAge);

	// What the pawn can detect
	Sight->DetectionByAffiliation.bDetectEnemies = true;
	Sight->DetectionByAffiliation.bDetectNeutrals = true;
	Sight->DetectionByAffiliation.bDetectFriendlies = true;

	// Configure the pawn's sight
	GetPerceptionComponent()->SetDominantSense(*Sight->GetSenseImplementation());

	// Called when sight is updated. Returns an Array of all the actors which updated the sight. 
	GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AGoblinAI::OnPawnDetected);

	//Called when sight is updated. Returns the target that updated it with the information about the sight updated. (Ex. Successfully sensed or not)
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AGoblinAI::OnTargetDetected);

	// Configura/Initialize the sight
	GetPerceptionComponent()->ConfigureSense(*Sight);

	// Create the AI components
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	// Set the values for the Blackboard Keys
	TargetEnemyKeyName = "TargetEnemy";
	PatrolLocationKeyName = "PatrolLocation";
	WaypointKeyName = "Waypoint";
	HasLineOfSightKeyName = "HasLineOfSight";
	CrystalKeyName = "Crystal";
}

void AGoblinAI::BeginPlay()
{
	Super::BeginPlay();

	ACrystal* ObjectToFind = nullptr;
	
	for (TActorIterator<ACrystal> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		Crystal = Cast<ACrystal>(*ActorItr);
		if (Crystal)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("This is an on screen message!"));
			SetCrystal(Crystal);
			return;
		}
	}
}

void AGoblinAI::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGoblin* Goblin = Cast<AGoblin>(InPawn);

	// If we are possessing something, try to starts its behavior tree
	if (Goblin)
	{
		if (Goblin->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*Goblin->BehaviorTree->BlackboardAsset);
		}

		BehaviorComp->StartTree(*Goblin->BehaviorTree);
	}
}

void AGoblinAI::OnUnPossess()
{
	Super::OnUnPossess();

	BehaviorComp->StopTree();
}

void AGoblinAI::OnPawnDetected(const TArray<AActor*>& DetectedPawns)
{
}

void AGoblinAI::OnTargetDetected(AActor* Actor, FAIStimulus const stimulus)
{
	// Check if the sensed actor is a player and if it was successfully sensed or not (i.e. entered or left the vision of the controller)
	AMainCharacter* Player = Cast<AMainCharacter>(Actor);
	bool isSensed = stimulus.WasSuccessfullySensed();

	// Check for nullptrs and update the keys in the blackboard
	if (Player)
	{
		if (isSensed)
		{
			SetTargetEnemy(Player);
			SetLineOfSight(isSensed);
		}
		else
		{
			SetTargetEnemy(nullptr);
			SetLineOfSight(isSensed);
		}
	}
}

FRotator AGoblinAI::GetControlRotation() const
{
	if (GetPawn() == nullptr)
		return FRotator(0.0f, 0.0f, 0.0f);

	return FRotator(0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f);
}

bool AGoblinAI::GetLineOfSight()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsBool(HasLineOfSightKeyName);
	}
	return nullptr;
}

void AGoblinAI::SetLineOfSight(bool value)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(HasLineOfSightKeyName, value);
	}
}

ABotWaypoint* AGoblinAI::GetWaypoint()
{
	if (BlackboardComp)
	{
		return Cast<ABotWaypoint>(BlackboardComp->GetValueAsObject(WaypointKeyName));
	}
	return nullptr;
}

void AGoblinAI::SetWaypoint(ABotWaypoint* NewWaypoint)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(WaypointKeyName, NewWaypoint);
	}
}

AMainCharacter* AGoblinAI::GetTargetEnemy()
{
	if (BlackboardComp)
	{
		return Cast<AMainCharacter>(BlackboardComp->GetValueAsObject(TargetEnemyKeyName));
	}
	return nullptr;
}

void AGoblinAI::SetTargetEnemy(APawn* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetEnemyKeyName, NewTarget);
	}
}

ACrystal* AGoblinAI::GetCrystal()
{
	if (BlackboardComp)
	{
		return Cast<ACrystal>(BlackboardComp->GetValueAsObject(CrystalKeyName));
	}
	return nullptr;
}

void AGoblinAI::SetCrystal(AActor* Target)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(CrystalKeyName, Target);
	}
}
