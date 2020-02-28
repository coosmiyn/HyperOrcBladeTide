// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapBase.h"
#include "Engine.h"
#include "Goblin.h"

// Sets default values
ATrapBase::ATrapBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnStepOn);

}

ATrapBase::ATrapBase(const FObjectInitializer& ObjectInitializer)
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	RootComponent = BoxComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComponent"));
	MeshComponent->SetupAttachment(BoxComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrapBase::OnStepOn);
}

// Called when the game starts or when spawned
void ATrapBase::BeginPlay()
{
	Super::BeginPlay();

	
	
}

void ATrapBase::OnStepOn(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AGoblin* Goblin = Cast<AGoblin>(OtherActor);
	if (Goblin)
	{
		GEngine->AddOnScreenDebugMessage(-3, 5.0f, FColor::Blue, TEXT("StepOn"));
		FPointDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = PunchDamageType;
		DamageEvent.Damage = 200.0f;

		Goblin->TakeDamage(DamageEvent.Damage, DamageEvent, NULL, this);
	}
}

// Called every frame
void ATrapBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATrapBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

