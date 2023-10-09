// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "AGP/Characters/BaseCharacter.h"
#include "AGP/Characters/HealthComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UWeaponComponent::SetWeaponStats(const FWeaponStats& NewWeaponStats)
{
	WeaponStats = NewWeaponStats;
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
}

bool UWeaponComponent::Fire(const FVector& BulletStart, FVector FireAtLocation)
{
	// checks
	if (TimeSinceLastShot < WeaponStats.FireRate)
		return false;
	if (RoundsRemainingInMagazine <= 0)
		return false;
	RoundsRemainingInMagazine -= 1;
	
	// add spread
	FVector Direction = FireAtLocation - BulletStart;
	Direction = Direction.RotateAngleAxis(180.0f*(1.0f-WeaponStats.Accuracy), FMath::VRand());
	FireAtLocation = BulletStart + Direction * FireAtLocation.Length();

	// fire raycast
	FHitResult OutHit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());
	if (GetWorld()->LineTraceSingleByChannel(OutHit, BulletStart,
		FireAtLocation, ECC_WorldStatic, Params))
	{
		if (ABaseCharacter* Character = Cast<ABaseCharacter>(OutHit.GetActor()))
		{
			DrawDebugLine(GetWorld(), BulletStart, OutHit.Location, FColor::Green, false,
				1, 0, 2);
			Character->GetComponentByClass<UHealthComponent>()->ApplyDamage(WeaponStats.BaseDamage);
		}
		else
		{
			DrawDebugLine(GetWorld(), BulletStart, OutHit.Location, FColor::Orange, false,
				1, 0, 2);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), BulletStart, FireAtLocation, FColor::Red, false,
			1, 0, 2);
	}

	TimeSinceLastShot = 0.0f;
	return true;
}

void UWeaponComponent::ReloadWeapon()
{
	if (RoundsRemainingInMagazine >= WeaponStats.MagazineSize) return;
	if (bReloading) return;

	bReloading = true;
	FTimerHandle Temp;
	GetOwner()->GetWorldTimerManager().SetTimer(Temp, this, &UWeaponComponent::Reload, WeaponStats.ReloadTime);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void UWeaponComponent::Reload()
{
	bReloading = false;
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
}

// Called every frame
void UWeaponComponent::TickComponent
	(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastShot += DeltaTime;
}

