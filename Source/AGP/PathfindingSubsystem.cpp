// Fill out your copyright notice in the Description page of Project Settings.


#include "PathfindingSubsystem.h"

#include "EngineUtils.h"
#include "NavigationNode.h"
#include <queue>

struct FAStarNode
{
	ANavigationNode* Node;
	FAStarNode* CameFrom;
	float G = std::numeric_limits<float>::max();
	float H;
	
	float GetF() const
	{
		return G + H;
	}

	bool operator>(const FAStarNode& Other) const
	{
		return GetF() > Other.GetF();
	}

	bool operator!=(const FAStarNode& Other) const
	{
		return this != nullptr;
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
	std::priority_queue<FAStarNode&, std::vector<FAStarNode>, std::greater<FAStarNode>> Queue;
	FAStarNode Start = {Node1, nullptr, 0, Node1->GetDistanceTo(Node2)};
	Queue.push(Start); // FACK

	TMap<ANavigationNode*, FAStarNode> Visited;
	Visited.Add(Node1, Start);

	while (!Queue.empty())
	{
		FAStarNode Current = Queue.top();
		Queue.pop();

		if (Current.Node == Node2)
		{
			return ReconstructPath(Current);
		}
		
		for (ANavigationNode* NeighbourNode : Current.Node->GetConnectedNodes())
		{
			FAStarNode Neighbour;
			float G = Current.G + Current.Node->GetDistanceTo(NeighbourNode);
			if (Visited.Contains(NeighbourNode)) 
			{
				// already visited
				Neighbour = Visited[NeighbourNode];

				// overwrite node if we have a better path to it
				if (Neighbour.G < G) continue;
				Neighbour.G = G;
				Neighbour.CameFrom = &Current;
			}
			else 
			{
				// new node
				Neighbour = FAStarNode{NeighbourNode, &Current, G,
					NeighbourNode->GetDistanceTo(Node2)};
				Visited.Add(NeighbourNode, Neighbour);
			}
		}	
	}
	
	return *new TArray<FVector>;
}

TArray<FVector> UPathfindingSubsystem::ReconstructPath(const FAStarNode& End)
{
	TArray<FVector> Path;
	Path.Add(End.Node->GetActorLocation());
	FAStarNode* Current = End.CameFrom;

	while (Current != nullptr)
	{
		Path.Add(Current->Node->GetActorLocation());
		Current = Current->CameFrom;
	}

	return Path; // it'll be in reverse
}
