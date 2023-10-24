// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerGameMode.h"

#include "AGPGameInstance.h"
#include "EngineUtils.h"
#include "Characters/BaseCharacter.h"
#include "Characters/EnemyCharacter.h"
#include "Characters/PlayerCharacter.h"
#include "GameFramework/PlayerStart.h"

void AMultiplayerGameMode::OnDeath(ABaseCharacter* Character)
{
	AController* Controller = Character->GetController();
	if (AEnemyCharacter* EnemyCharacter = Cast<AEnemyCharacter>(Controller->GetPawn()))
	{
		Character->Destroy();
		
		// select spawn location
		UAGPGameInstance* GameInstance = GetWorld()->GetGameInstance<UAGPGameInstance>();
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			FVector Location = (*It)->GetActorLocation();
			GetWorld()->SpawnActor<AEnemyCharacter>(GameInstance->GetEnemyCharacterClass(), Location, FRotator::ZeroRotator);
			break;
		}
	}
	else
	{
		Character->Destroy();
		RestartPlayer(Controller);
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Controller->GetPawn()))
			PlayerCharacter->BeginPlayImplementation();
	}
}
