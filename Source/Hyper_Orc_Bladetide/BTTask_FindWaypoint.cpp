// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindWaypoint.h"
#include "GoblinAI.h"
#include "BotWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Engine.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_FindWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//return EBTNodeResult::Type();

	AGoblinAI* Controller = Cast<AGoblinAI>(OwnerComp.GetAIOwner());

	if (Controller == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ABotWaypoint* Waypoint = Controller->GetWaypoint();
	AActor* NewWaypoint = nullptr;

	TArray<AActor*> AllWaypoints;

	UGameplayStatics::GetAllActorsOfClass(Controller, ABotWaypoint::StaticClass(), AllWaypoints);

	if (AllWaypoints.Num() == 0)
	{
		return EBTNodeResult::Failed;
	}

	NewWaypoint = AllWaypoints[FMath::RandRange(0, AllWaypoints.Num() - 1)];

	if (NewWaypoint)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), NewWaypoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}

