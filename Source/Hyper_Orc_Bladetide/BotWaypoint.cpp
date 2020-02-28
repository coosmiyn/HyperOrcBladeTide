// Fill out your copyright notice in the Description page of Project Settings.


#include "BotWaypoint.h"
#include "Goblin.h"

// Sets default values
ABotWaypoint::ABotWaypoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(Root);

	// Set up collision
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxCollider->SetupAttachment(GetRootComponent());

	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ABotWaypoint::OnPlayerEnter);

}

ABotWaypoint* ABotWaypoint::GetNextWaypoint()
{
	return NextWaypoint;
}

// Called when the game starts or when spawned
void ABotWaypoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABotWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// When a Character enters the collision box set the next waypoint he should go to
void ABotWaypoint::OnPlayerEnter(UPrimitiveComponent* OverlapComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, 
									int32 OtherBoxyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGoblin* Character = nullptr;

	if (OtherActor != nullptr)
	{
		Character = Cast<AGoblin>(OtherActor);
		if (Character != nullptr)
			Character->NextWaypoint = NextWaypoint;
	}
}

