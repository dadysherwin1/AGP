// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponComponent.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle,
	Pistol
};

USTRUCT(BlueprintType)
struct FWeaponStats
{
	GENERATED_BODY()
	
public:
	EWeaponType WeaponType = EWeaponType::Rifle;
	float Accuracy = 1.0f;
	float FireRate = 0.2f;
	float BaseDamage = 10.0f;
	uint32 MagazineSize = 5;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AGP_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	void Fire(const FVector& BulletStart, const FVector& FireAtLocation);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FWeaponStats WeaponStats;
	uint32 RoundsRemainingInMagazine;
	float TimeSinceLastShot = 0.0f;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool FireImplementation(const FVector& BulletStart, const FVector& FireAtLocation, FVector& OutHitLocation);
	void FireVisualImplementation(const FVector& BulletStart, const FVector& HitLocation);
	UFUNCTION(NetMulticast, Unreliable)
	void MulticastFire(const FVector& BulletStart, const FVector& HitLocation);
};
