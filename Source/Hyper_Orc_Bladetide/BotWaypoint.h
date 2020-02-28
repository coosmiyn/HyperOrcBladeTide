// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BotWaypoint.generated.h"

UCLASS()
class HYPER_ORC_BLADETIDE_API ABotWaypoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABotWaypoint();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* Root;

	// Box collision
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* BoxCollider;

	// A pointer to the next waypoint
	// Currently unused because the AI has/will have decision making and will not need to manually grab these
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ABotWaypoint* NextWaypoint;

	ABotWaypoint* GetNextWaypoint();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
							int32 OtherBoxyIndex, bool bFromSweep, const FHitResult &SweepResult);

};
