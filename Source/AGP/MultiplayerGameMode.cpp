// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "Characters/BaseCharacter.h"
#include "Characters/PlayerCharacter.h"

void AMultiplayerGameMode::OnDeath(ABaseCharacter* Character)
{
	AController* Controller = Character->GetController();
	Character->Destroy();
	RestartPlayer(Controller);
	if (APlayerCharacter* NewCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
		NewCharacter->BeginPlayImplementation();
}
