// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BulletStartPosition = CreateDefaultSubobject<USceneComponent>(TEXT("BulletStartPosition"));
	BulletStartPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ABaseCharacter::Fire(const FVector& FireAtLocation)
{
	if (TimeSinceLastShot < MinTimeBetweenShots)
	{
		return false;
	}

	FHitResult OutHit;
	const FVector Start = GetActorLocation();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start,
		FireAtLocation, ECC_WorldStatic, Params))
	{
		if (Cast<ABaseCharacter>(OutHit.GetActor()))
		{
			DrawDebugLine(GetWorld(), Start, OutHit.Location, FColor::Green, false, 1, 0, 2);
		}
		else
		{
			DrawDebugLine(GetWorld(), Start, OutHit.Location, FColor::Orange, false, 1, 0, 2);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, FireAtLocation, FColor::Red, false, 1, 0, 2);
	}

	TimeSinceLastShot = 0.0f;
	return true;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasWeapon())
		TimeSinceLastShot += DeltaTime;
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

bool ABaseCharacter::HasWeapon()
{
	return bHasWeaponEquipped;
}

void ABaseCharacter::EquipWeapon(bool bEquipGun)
{
	bHasWeaponEquipped = bEquipGun;
	if (bEquipGun)
	{
		UE_LOG(LogTemp, Display, TEXT("Player has equipped weapon"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("Player has unequipped weapon"));
	}
	
	OnWeaponEquip(bEquipGun);
}

