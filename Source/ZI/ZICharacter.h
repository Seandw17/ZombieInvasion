// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZICharacter.generated.h"

UCLASS(config=Game)
class AZICharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sphere", meta = (AllowPrivateAccess = "true"))
		class USphereComponent* CollectionSphere;
	
public:
	AZICharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* RifleMesh;
	
	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USceneComponent* MuzzleLocation;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* CaseLocation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class AProjectile> ProjectileClass;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABulletCasing> BulletCasingClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* ReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* PlayerReloadAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* PlayerFireAnimation;
	
	UPROPERTY()
	FTimerHandle FireTimerHandle;

	UPROPERTY()
	FTimerHandle ReloadTimerHandle;

protected:

	virtual void BeginPlay() override ;

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	UFUNCTION(BlueprintCallable, Category = "PickUps")
	void CollectPickups();

	//Start Firing Rifle
	void StartFire();

	//Stop Firing Rifle
	void StopFire();

	//Reload Rifle
	void Reload();

	void StopReloading();

	//Function to change between watch towers
	void SwapTowers();


	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

public:
	//Bool to see if the weapon is able to fire full auto 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FullAutoBool")
		bool FullAuto;
	//Bool to see if thge player is reloading
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ReloadingBool")
		bool IsReloading;
	//Bool to see if the player is able to fire
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "CanFireBool")
		bool AbletoFire;
	//integer for the watch towers
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "TowerNumber")
		int TowerNumber;
	//Max ammo for the magazine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int MaxMagazineAmmo;
	//Ammo for the magazine
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int MagazineAmmo;
	//Max Ammo player can have
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int MaxAmmo;
	//Ammo the player has
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
		int Ammo;
	//Player Kill count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player Kill Count")
		int PlayerKillCount;
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	FORCEINLINE class USphereComponent* GetCollectionSphere() const { return CollectionSphere; }
};

