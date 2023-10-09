// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupBase.h"
#include "WeaponComponent.h"
#include "WeaponPickup.generated.h"

UENUM(BlueprintType)
enum class EWeaponRarity : uint8
{
	Common,
	Rare,
	Master,
	Legendary
};

UCLASS()
class AGP_API AWeaponPickup : public APickupBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	void OnPickupOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(BlueprintReadOnly) EWeaponRarity WeaponRarity = EWeaponRarity::Common;
	FWeaponStats WeaponStats;
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateWeaponPickupMaterial();

private:
	EWeaponRarity GenerateRarity();
	void GenerateWeaponPickup();

	UPROPERTY(EditDefaultsOnly) FVector2f BadAccuracy;
	UPROPERTY(EditDefaultsOnly) FVector2f GoodAccuracy;
	UPROPERTY(EditDefaultsOnly) FVector2f BadFireRate;
	UPROPERTY(EditDefaultsOnly) FVector2f GoodFireRate;
	UPROPERTY(EditDefaultsOnly) FVector2f BadBaseDamage;
	UPROPERTY(EditDefaultsOnly) FVector2f GoodBaseDamage;
	UPROPERTY(EditDefaultsOnly) FIntVector2 BadMagazineSize;
	UPROPERTY(EditDefaultsOnly) FIntVector2 GoodMagazineSize;
	UPROPERTY(EditDefaultsOnly) FVector2f BadReloadTime;
	UPROPERTY(EditDefaultsOnly) FVector2f GoodReloadTime;
	float GenerateAccuracy(const bool& bIsGood);
	float GenerateFireRate(const bool& bIsGood);
	float GenerateBaseDamage(const bool& bIsGood);
	uint32 GenerateMagazineSize(const bool& bIsGood);
	float GenerateReloadTime(const bool& bIsGood);
	void PickRandomStat(TArray<bool>& GoodStats);
};