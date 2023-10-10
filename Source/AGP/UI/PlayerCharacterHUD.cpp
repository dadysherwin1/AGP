// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacterHUD.h"

#include "Components/ProgressBar.h"

void UPlayerCharacterHUD::SetHealthBar(float HealthPercent)
{
	HealthBar->SetPercent(HealthPercent);
}
