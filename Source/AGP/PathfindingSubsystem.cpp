// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingSubsystem.h"

#include "EngineUtils.h"
#include "NavigationNode.h"
#include <queue>

struct FAStarNode
{
	float G = std::numeric_limits<float>::max();;
	float H = 0;
	float GetF() const
	{
		return G + H;
	}
	ANavigationNode CameFrom;

	bool operator>(const FAStarNode& Other) const
	{
		return GetF() > Other.GetF();
	}
};

void UPathfindingSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	PopulateNodes();
	GetPath(GetRandomNode(), GetRandomNode());
}

TArray<FVector> UPathfindingSubsystem::GetRandomPath(const FVector& StartLocation)
{
	return *new TArray<FVector>;
}

void UPathfindingSubsystem::PopulateNodes()
{
	Nodes.Empty();

	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		Nodes.Add(*It);
		UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *(*It)->GetActorNameOrLabel(), *(*It)->GetActorLocation().ToString());
	}
}

ANavigationNode* UPathfindingSubsystem::GetRandomNode()
{
	if (Nodes.IsEmpty())
		return nullptr;
	else
	{
		int Index = FMath::RandRange(0, Nodes.Num()-1);
		return Nodes[Index];
	}
}

ANavigationNode* UPathfindingSubsystem::FindNearestNode(const FVector& TargetLocation)
{
	ANavigationNode* ClosestNode = nullptr;
	float ClosestDistance = std::numeric_limits<float>::max();
	for (ANavigationNode* Node : Nodes)
	{
		float Distance = FVector::Distance(TargetLocation, Node->GetActorLocation());
		
		if (Distance < ClosestDistance)
		{
			ClosestNode = Node;
			ClosestDistance = Distance;
		}
	}

	return ClosestNode;
}

TArray<FVector> UPathfindingSubsystem::GetPath(ANavigationNode* Node1, ANavigationNode* Node2)
{
	// std::priority_queue<FAStarNode, std::vector<FAStarNode>, std::greater<FAStarNode>> Queue;
	FAStarNode Yo {3,3};
	return *new TArray<FVector>;
}
