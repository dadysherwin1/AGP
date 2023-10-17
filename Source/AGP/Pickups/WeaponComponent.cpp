// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponComponent.h"

#include "AGP/AGPGameInstance.h"
#include "Net/UnrealNetwork.h"
#include "AGP/Characters/BaseCharacter.h"
#include "AGP/Characters/HealthComponent.h"
#include "AGP/Characters/PlayerCharacter.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
	// ...
}

void UWeaponComponent::SetWeaponStats(const FWeaponStats& NewWeaponStats)
{
	WeaponStats = NewWeaponStats;
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
	OnWeaponStatsChanged();
	OnAmmoChanged();
}

void UWeaponComponent::Fire(const FVector& BulletStart, FVector FireAtLocation)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		FVector HitLocation;
		int HitType;
		if (FireImplementation(BulletStart, FireAtLocation, HitLocation, HitType))
		{
			OnAmmoChanged();
			MulticastFire(BulletStart, HitLocation, HitType);
		}
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy)
		ServerFire(BulletStart, FireAtLocation);
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UWeaponComponent::ReloadWeapon()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ReloadImplementation();
		MulticastReload();
	}
	else if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		ServerReload();
	}
}

void UWeaponComponent::Reload()
{
	bReloading = false;
	RoundsRemainingInMagazine = WeaponStats.MagazineSize;
	OnAmmoChanged();
}

void UWeaponComponent::ReloadImplementation()
{
	if (RoundsRemainingInMagazine >= WeaponStats.MagazineSize) return;
	if (bReloading) return;

	bReloading = true;
	FTimerHandle Temp;
	GetOwner()->GetWorldTimerManager().SetTimer(Temp, this, &UWeaponComponent::Reload, WeaponStats.ReloadTime);
}

void UWeaponComponent::OnAmmoChanged()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->UpdateAmmoText(RoundsRemainingInMagazine);
	}
}

void UWeaponComponent::OnWeaponStatsChanged()
{
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->UpdateMagazineSizeText(WeaponStats.MagazineSize);
	}
}

void UWeaponComponent::MulticastReload_Implementation()
{
	ReloadImplementation();
}

void UWeaponComponent::ServerReload_Implementation()
{
	ReloadImplementation();
	MulticastReload();
}

bool UWeaponComponent::FireImplementation(const FVector& BulletStart, FVector FireAtLocation,
	FVector& OutHitLocation, int& OutHitType)
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
			//DrawDebugLine(GetWorld(), BulletStart, OutHit.Location, FColor::Green, false,
			//	1, 0, 2);
			Character->GetComponentByClass<UHealthComponent>()->ApplyDamage(WeaponStats.BaseDamage);
			OutHitType = 2;
		}
		else
		{
			//DrawDebugLine(GetWorld(), BulletStart, OutHit.Location, FColor::Orange, false,
			//	1, 0, 2);
			OutHitType = 1;
		}
		OutHitLocation = OutHit.Location;
	}
	else
	{
		//DrawDebugLine(GetWorld(), BulletStart, FireAtLocation, FColor::Red, false,
		//	1, 0, 2);
		OutHitLocation = FireAtLocation;
		OutHitType = 0;
	}

	TimeSinceLastShot = 0.0f;
	return true;
}

void UWeaponComponent::FireVisualImplementation(const FVector& BulletStart, const FVector& HitLocation, const int& HitType)
{
	// DrawDebugLine(GetWorld(), BulletStart, HitLocation, FColor::Blue, false, 1);
	UAGPGameInstance* GameInstance = Cast<UAGPGameInstance>(GetWorld()->GetGameInstance());
	if (HitType > 0)
	{
		if (HitType == 1)
			GameInstance->SpawnGroundHitParticles(HitLocation);
		else if (HitType == 2)
			GameInstance->SpawnBloodParticles(HitLocation);
	}
	if (ABaseCharacter* Character = Cast<ABaseCharacter>(GetOwner()))
	{
		if (Character->IsLocallyControlled())
			GameInstance->PlayGunshotSound2D();
		else
			GameInstance->PlayGunshotSoundAtLocation(BulletStart);
		Character->FireWeaponGraphical();
	}
		
}

void UWeaponComponent::ServerFire_Implementation(const FVector& BulletStart, const FVector& FireAtLocation)
{
	FVector HitLocation;
	int HitType;
	if (FireImplementation(BulletStart, FireAtLocation, HitLocation, HitType))
		MulticastFire(BulletStart, HitLocation, HitType);
}

void UWeaponComponent::MulticastFire_Implementation(const FVector& BulletStart, const FVector& HitLocation, const int& HitType)
{
	FireVisualImplementation(BulletStart, HitLocation, HitType);
}

// Called every frame
void UWeaponComponent::TickComponent
	(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TimeSinceLastShot += DeltaTime;
}

void UWeaponComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UWeaponComponent, WeaponStats);
	DOREPLIFETIME(UWeaponComponent, RoundsRemainingInMagazine);
}

