// Fill out your copyright notice in the Description page of Project Settings.


#include "ProceduralLandscape.h"

#include "EngineUtils.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "KismetProceduralMeshLibrary.h"
#include "Pathfinding/NavigationNode.h"

// Sets default values
AProceduralLandscape::AProceduralLandscape()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Procedural Mesh"));
	SetRootComponent(ProceduralMesh);
}

void AProceduralLandscape::ClearLandscape()
{
	Vertices.Empty();
	Triangles.Empty();
	UVCoords.Empty();

	ProceduralMesh->ClearMeshSection(0);
	UKismetSystemLibrary::FlushPersistentDebugLines(GetWorld());

	for (TActorIterator<ANavigationNode> It(GetWorld()); It; ++It)
	{
		It->Destroy();
	}
}

// Called when the game starts or when spawned
void AProceduralLandscape::BeginPlay()
{
	Super::BeginPlay();
}

void AProceduralLandscape::CreateSimplePlane()
{

	// add vertices
	Vertices.Add(FVector(0,0,FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Add(FVector(1000.0f,0,FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Add(FVector(0,1000.0f,FMath::RandRange(-500.0f, 500.0f)));
	Vertices.Add(FVector(1000.0f,1000.0f,FMath::RandRange(-500.0f, 500.0f)));
	for (const FVector Vertex : Vertices)
	{
		DrawDebugSphere(GetWorld(), Vertex, 50.0f, 4, FColor::Cyan, true);
	}

	// add triangles
	Triangles.Add(0);
	Triangles.Add(2);
	Triangles.Add(1);
	Triangles.Add(1);
	Triangles.Add(2);
	Triangles.Add(3);

	// add uv coords
	UVCoords.Add(FVector2D(0.0f, 0.0f));
	UVCoords.Add(FVector2D(1.0f, 0.0f));
	UVCoords.Add(FVector2D(0.0f, 1.0f));
	UVCoords.Add(FVector2D(1.0f, 1.0f));

	// create mesh
	if (ProceduralMesh)
	{
		ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, TArray<FVector>(), UVCoords,
			TArray<FColor>(), TArray<FProcMeshTangent>(), true);
	}
};

// Called every frame
void AProceduralLandscape::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bShouldRegenerate)
	{
		ClearLandscape();
		// CreateSimplePlane();
		GenerateLandscape();
		bShouldRegenerate = false;
	}
}

bool AProceduralLandscape::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AProceduralLandscape::GenerateLandscape()
{
	
	PerlinOffset = FMath::RandRange(-1'000'000.0f, 1'000'000.0f); // random seed
	TArray<ANavigationNode*> Nodes;
	

	// add vertices & UV
	for (int32 Y = 0; Y < Height; Y++)
	{
		for (int32 X = 0; X < Width; X++)
		{
			// vertex
			float XPos = X * VertexSpacing;
			float YPos = Y * VertexSpacing;
			float ZPos = FMath::PerlinNoise2D(FVector2D(XPos * PerlinRoughness + PerlinOffset,
					YPos * PerlinRoughness + PerlinOffset)) * PerlinScale;
			const FVector Vertex = FVector(XPos, YPos, ZPos);
			Vertices.Add(Vertex);
			// DrawDebugSphere(GetWorld(), Vertex, 50.0f, 4, FColor::Cyan, true);

			// UV
			UVCoords.Add(FVector2D(X, Y));

			// Nodes
			ANavigationNode* Node = GetWorld()->SpawnActor<ANavigationNode>(FVector(XPos, YPos, ZPos), FRotator());
			Nodes.Add(Node);
		}
	}

	// add triangles & navigation nodes
	for (int32 Y = 0; Y < Height-1; Y++)
	{
		for (int32 X = 0; X < Width-1; X++)
		{
			const int32 BR = Y * Width + X;
			const int32 TR = (Y+1) * Width + X;
			const int32 BL = Y * Width + (X+1);
			const int32 TL = (Y+1) * Width + (X+1);
			Triangles.Add(BR);
			Triangles.Add(TR);
			Triangles.Add(BL);
			Triangles.Add(BL);
			Triangles.Add(TR);
			Triangles.Add(TL);

			// Navigation Node
			Nodes[BR]->ConnectToNode(Nodes[BL]);
			Nodes[BR]->ConnectToNode(Nodes[TR]);
			Nodes[BR]->ConnectToNode(Nodes[TL]);
			Nodes[BL]->ConnectToNode(Nodes[TR]);
			Nodes[BL]->ConnectToNode(Nodes[TL]);
			Nodes[TR]->ConnectToNode(Nodes[TL]);
		}
	}

	TArray<FVector> Normals;
	TArray<FProcMeshTangent> Tangents;
	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(Vertices, Triangles, UVCoords, Normals, Tangents);

	// create mesh
	ProceduralMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVCoords,
		TArray<FColor>(), Tangents, true);
}

