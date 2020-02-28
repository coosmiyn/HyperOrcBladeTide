// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "TrapBase.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Disable the rotation cotrols for the character mesh so it is used only by the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Allow the character to be rotated
	GetCharacterMovement()->bOrientRotationToMovement = true;

	// Set the movement variables
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Create the Arm Component which will hold the camera and attach it to the player
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	// Set the length of the arm and allow it to rotate
	CameraBoom->TargetArmLength = 50.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create the camera for the player and attach it to the Spring Arm component
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Disable rotation for the camera since it is attached to the Spring Arm component which will rotate.
	FollowCamera->bUsePawnControlRotation = false;

	// Create the projectile and set it's location relative spawn to the player
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));
	FP_MuzzleLocation->SetupAttachment(RootComponent);

	// Set the location where the the gun will be placed (not used at the moment);
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Needs to be called from an external source to add damage to the player
// TO DO: Implement the Unreal version which has more components and can be overriden
void AMainCharacter::TakeDamage2(float Damage)
{
	HealthPoints -= Damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("damage, health = %f"), HealthPoints));

	// TO DO: Create a die method
	if (HealthPoints <= 0)
	{
		// Disable the collision detectors
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		USkeletalMeshComponent* skeletal = GetMesh();
		if (skeletal)
		{
			// Change the physics and collision for the mesh to create a "death" effect
			skeletal->SetCollisionProfileName(TEXT("Ragdoll")); 
			skeletal->SetAllBodiesSimulatePhysics(true);
			skeletal->SetSimulatePhysics(true);
			skeletal->WakeAllRigidBodies();
			skeletal->bBlendPhysics = true;
			
			// Detach the camera and invalidate control imputs
			CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
			CameraBoom->bUsePawnControlRotation = false;
			CameraBoom->SetupAttachment(skeletal);
		}
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAction("PickUpObject", IE_Pressed, this, &AMainCharacter::PickUpObject);

	PlayerInputComponent->BindAxis("MoveObjectX", this, &AMainCharacter::MoveObjectX);
	PlayerInputComponent->BindAxis("MoveObjectY", this, &AMainCharacter::MoveObjectY);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::OnFire);

}

void AMainCharacter::MoveForward(float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(Direction, Value);
}

AActor* AMainCharacter::Raycast()
{
	FHitResult OutHit;

	FVector Start = FollowCamera->GetComponentLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();

	Start = Start + (ForwardVector * CameraBoom->TargetArmLength);
	FVector End = Start + (ForwardVector * 2000.0f);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this->GetOwner());

	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams);

	if (isHit)
	{
		/*if (OutHit.GetActor()->ActorHasTag("Destroy"))
		{
			OutHit.GetActor()->Destroy();
		}*/

		return OutHit.GetActor();
	}
	else
	{
		return nullptr;
	}
}

void AMainCharacter::OnFire()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GetControlRotation();

			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

}

void AMainCharacter::PickUpObject()
{
	isPickingObject = !isPickingObject;

	// Check if the player is holding anything at the moment
	if (HeldComponent == nullptr)
	{
		// Get the object the player is aiming at
		AActor* PickableObject = Raycast();

		// Check if the Raycast returned something and if it is a "movable" object
		// TO DO: Add class for pickable objecets and check for class instead of tag
		if (PickableObject != nullptr && PickableObject->ActorHasTag("Movable"))
		{
			isPickingObject = true;
			HeldComponent = PickableObject;

			// Store the initial position in case the object is put in an invalid location
			HeldComponentInitialPosition = PickableObject->GetActorLocation();


			/*FVector cameraLocation = FollowCamera->GetComponentLocation();
			FVector OffsetLocation = cameraLocation + (FollowCamera->GetForwardVector() * 100.0f);
			PickableObject->SetActorRelativeLocation(OffsetLocation);*/


			/*UStaticMeshComponent* HeldObject = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HeldObject"));
			HeldObject->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);*/
		}
	}
	else
	{
		isPickingObject = false;
		HeldComponent = nullptr;
	}
}

void AMainCharacter::MoveObjectX(float Value)
{
	if (HeldComponent != nullptr)
	{
		AActor* SeenObject = Raycast();
		if (SeenObject != nullptr)
		{
			// Check if the seen object can store an object/trap
			// TO DO: Add a class for tiles and check the class not the tag
			if (SeenObject->ActorHasTag("Place"))
			{
				FVector tileLocation = SeenObject->GetActorLocation();
				
				// The location is at the center of the object so it needs to be corrected
				//tileLocation += FVector(0.0f, 0.0f, tileLocation.Z + 325.0f);
				//tileLocation += FVector(0.0f, 0.0f, tileLocation.Z);
				HeldComponent->SetActorLocation(tileLocation);
			}
			else
			{
				HeldComponent->SetActorLocation(HeldComponentInitialPosition);
			}
		}
	}
}

void AMainCharacter::MoveObjectY(float Value)
{
	if (HeldComponent != nullptr)
	{
		AActor* SeenObject = Raycast();
		if (SeenObject != nullptr)
		{
			// Check if the seen object can store an object/trap
			// TO DO: Add a class for tiles and check the class not the tag
			if (SeenObject->ActorHasTag("Place"))
			{
				FVector tileLocation = SeenObject->GetActorLocation();

				// The location is at the center of the object so it needs to be corrected
				//tileLocation += FVector(0.0f, 0.0f, tileLocation.Z + 325.0f);
				//tileLocation += FVector(0.0f, 0.0f, tileLocation.Z);
				HeldComponent->SetActorLocation(tileLocation);
			}
			else
			{
				HeldComponent->SetActorLocation(HeldComponentInitialPosition);
			}
		}
	}
}

