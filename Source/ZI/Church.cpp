// Fill out your copyright notice in the Description page of Project Settings.

#include "Church.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZICharacter.h"
#include "Projectile.h"
#include "Enemy.h"
#include "ZIGameMode.h"
#include "Engine/World.h"

// Sets default values
AChurch::AChurch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Churchmesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Church"));
	RootComponent = Churchmesh;

	ChurchSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ChurchSphereCollision"));
	ChurchSphereCollision->InitSphereRadius(750.f);
	ChurchSphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AChurch::OnOverlapBegin);
	ChurchSphereCollision->SetupAttachment(Churchmesh);


	MaxHealth = 100.f;
	Health = MaxHealth;
	
}

// Called when the game starts or when spawned
void AChurch::BeginPlay()
{
	Super::BeginPlay();
	IsAlive = true;
}

// Called every frame
void AChurch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Health <= 0.f)
	{
		Destroy();
		DecreaseBuildings();
		IsAlive = false;
	}

}

void AChurch::DecreaseBuildings()
{
	AZIGameMode* Mode = Cast<AZIGameMode>(GetWorld()->GetAuthGameMode());
	if (Mode != NULL)
	{
		Mode->DecreaseBuildings();
	}
}

float AChurch::GetHealth()
{
	return Health;
}

void AChurch::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Zombie = Cast<AEnemy>(OtherActor);
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		if (OtherActor == Zombie)
		{
			Health -= 20.f;
			Zombie->Sacrafice();
		}
	}
}

void AChurch::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}