// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "PathfindingSubsystem.generated.h"

struct FAStarNode;
class ANavigationNode;
/**
 * 
 */
UCLASS()
class AGP_API UPathfindingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	
	TArray<FVector> GetRandomPath(const FVector& StartLocation);

protected:
	UPROPERTY()
	TArray<ANavigationNode*> Nodes;

private:
	void PopulateNodes();
	
	ANavigationNode* GetRandomNode();
	ANavigationNode* FindNearestNode(const FVector& TargetLocation);
	TArray<FVector> GetPath(ANavigationNode* Node1, ANavigationNode* Node2);
	TArray<FVector> ReconstructPath(const FAStarNode& End);
};
