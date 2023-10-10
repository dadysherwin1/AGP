// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UPlayerCharacterHUD::SetHealthBar(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
}

void UPlayerCharacterHUD::SetAmmoText(int Ammo)
{
	AmmoText->SetText(FText::FromString(FString::FromInt(Ammo)));
}

void UPlayerCharacterHUD::SetMagazineSizeText(int MagazineSize)
{
	MagazineSizeText->SetText(FText::FromString(FString::FromInt(MagazineSize)));
}
