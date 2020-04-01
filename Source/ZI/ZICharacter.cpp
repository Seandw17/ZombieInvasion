// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "ZICharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "BulletCasing.h"
#include "AmmoPickUp.h"

//////////////////////////////////////////////////////////////////////////
// AZICharacter

AZICharacter::AZICharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	//FollowCamera->SetupAttachment(RootComponent);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
	RifleMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Rifle"));
	RifleMesh->SetupAttachment(RootComponent);
	
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(RifleMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	CaseLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CaseLocation"));
	CaseLocation->SetupAttachment(RifleMesh);
	CaseLocation->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	CollectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollectionSphere"));
	CollectionSphere->SetupAttachment(RootComponent);
	CollectionSphere->SetSphereRadius(200.f);

	FullAuto = false;
	AbletoFire = true;
	TowerNumber = 0;

	MaxAmmo = 5;
	Ammo = MaxAmmo;

	MaxMagazineAmmo = 30;
	MagazineAmmo = MaxMagazineAmmo;
	
	PlayerKillCount = 0;
}


//Begin Play
void AZICharacter::BeginPlay()
{
	Super::BeginPlay();
	RifleMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("Rifle"));
}

//////////////////////////////////////////////////////////////////////////
// Input

void AZICharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AZICharacter::Reload);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZICharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZICharacter::StopFire);

	PlayerInputComponent->BindAction("Swap", IE_Pressed, this, &AZICharacter::SwapTowers);

	PlayerInputComponent->BindAction("Collect", IE_Pressed, this, &AZICharacter::CollectPickups);

	PlayerInputComponent->BindAxis("MoveForward", this, &AZICharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AZICharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AZICharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AZICharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AZICharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AZICharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AZICharacter::OnResetVR);
}


void AZICharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AZICharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AZICharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AZICharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AZICharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AZICharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AZICharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AZICharacter::Reload()
{

		IsReloading = true;
		UAnimInstance* RifleAnimation = RifleMesh->GetAnimInstance();
		UAnimInstance* PlayerAnimation = GetMesh()->GetAnimInstance();
		if (RifleAnimation != NULL || PlayerAnimation != NULL)
		{
			RifleAnimation->Montage_Play(ReloadAnimation, 1.f);
			PlayerAnimation->Montage_Play(PlayerReloadAnimation, 1.f);
		}
		MagazineAmmo = 30;
		Ammo--;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AZICharacter::StopReloading, 2.0f, false);
	
	 if(Ammo <= 0)
	 {
		Ammo = 0;
		IsReloading = false;
	 }

	 GetCharacterMovement()->MaxWalkSpeed = 0.0;
	
}

void AZICharacter::StopReloading()
{
	IsReloading = false;
	GetCharacterMovement()->MaxWalkSpeed = 300.0;
}

void AZICharacter::StartFire()
{
	if (IsReloading == false)
	{
		if (Ammo >= 0)
		{

			if (ProjectileClass != NULL)
			{
				UWorld* const World = GetWorld();
				if (World != NULL)
				{

					const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
					const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
					World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

					const FRotator CaseRotation = CaseLocation->GetComponentRotation();
					const FVector CaseSpawnLocation = ((CaseLocation != nullptr) ? CaseLocation->GetComponentLocation() : GetActorLocation()) + CaseRotation.RotateVector(GunOffset);
					World->SpawnActor<ABulletCasing>(BulletCasingClass, CaseSpawnLocation, CaseRotation, ActorSpawnParams);
;

					MagazineAmmo--;
				}

			}
			

			UAnimInstance* RifleAnimation = RifleMesh->GetAnimInstance();
			UAnimInstance* PlayerAnimation = GetMesh()->GetAnimInstance();
			if (RifleAnimation != NULL || PlayerAnimation != NULL)
			{
				RifleAnimation->Montage_Play(FireAnimation, 1.f);
				PlayerAnimation->Montage_Play(PlayerFireAnimation, 1.f);
			}

			if (FullAuto == true)
			{
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AZICharacter::StartFire, 0.2f, true);
			}
			if (MagazineAmmo <= 0 && Ammo >= 0)
			{
				
				AbletoFire = false;
				Reload();
			}
		}
	}
}

void AZICharacter::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void AZICharacter::SwapTowers()
{
	TowerNumber++;
	if (TowerNumber >= 4)
	{
		TowerNumber = 0;
	}

	switch (TowerNumber)
	{
	case 0:
	{
		SetActorLocation(FVector(3705, 2860, 1355));
	}
	break;
	case 1:
	{
		SetActorLocation(FVector(-660, -2535, 1355));
	}
	break;
	case 2:
	{
		SetActorLocation(FVector(-1605, 795, 1355));
	}
	break;
	default:
	{
		SetActorLocation(FVector(4380, -1975, 1355));
	}
		break;
	}
}

void AZICharacter::CollectPickups()
{
	//Get all overlapping Actors and store them in an array
	TArray<AActor*> CollectedActors;
	CollectionSphere->GetOverlappingActors(CollectedActors);

	// For each Actor we collected
	for (int32 iCollected = 0; iCollected < CollectedActors.Num(); ++iCollected)
	{
		AAmmoPickUp* const AmmoPickup = Cast<AAmmoPickUp>(CollectedActors[iCollected]);
		//If the cast is successful and the pickup is valid and active
		if (AmmoPickup && !AmmoPickup->IsPendingKill() && AmmoPickup->IsActive())
		{
			//Call the pickup IsCollected Function
			AmmoPickup->IsCollected();
			//Deactivate the pickup
			AmmoPickup->SetActive(false);
		}
	}
}