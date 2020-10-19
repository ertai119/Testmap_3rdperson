// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Generator/MeshGenerator.h"
#include "../Generator/RoomGenerator.h"
#include "RandomMapGenActor.generated.h"


UCLASS()
class ARandomMapGenActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomMapGenActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

    void InitializeMap();
    void DeInitializeMap();

    void GenerateMesh(const TArray<TArray<int32>>& map);
    void CreateFloor();
    FVector CoordToWorldPosition(const Coord& coord) const;

    void CreateToStaticMesh(int32 sectionIndex);

    UPROPERTY(EditAnywhere)
    UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere)
    UStaticMesh* WallMesh;

    UPROPERTY(EditAnywhere)
    bool Generate;

    UPROPERTY(EditAnywhere)
    bool Simulate;

	UPROPERTY(EditAnywhere)
	int32 SimulateCount = 2;

    UPROPERTY(EditAnywhere)
    int32 BorderSize = 1;

    UPROPERTY(EditAnywhere)
    float SquareSize = 30.f;

	UPROPERTY(EditAnywhere)
	int32 Width = 80;

    UPROPERTY(EditAnywhere)
    int32 Height = 50;

    UPROPERTY(EditAnywhere)
    int32 WallHeight = 50;

    UPROPERTY(EditAnywhere)
    int32 PassageSize = 3;

    UPROPERTY(EditAnywhere)
    float SpawnProbability = 45.f;

    UPROPERTY(EditAnywhere)
    int32 WallThreshold = 50;

    UPROPERTY(EditAnywhere)
    int32 RoomThreshold = 50;

	UPROPERTY(EditAnywhere)
	int32 WallThresholdAfterProcessed = 50;

	UPROPERTY(EditAnywhere)
	int32 RoomThresholdAfterProcessed = 50;

    UPROPERTY(EditAnywhere)
    int32 DeathCount = 4;

    UPROPERTY(EditAnywhere)
    int32 BirthCount = 4;

protected:

    UPROPERTY(EditAnywhere)
    class UProceduralMeshComponent* proceduralMeshComp_;

    UPROPERTY(EditAnywhere)
    TMap<int32, class UStaticMeshComponent*> spawnedStaticMeshComps_;

	UPROPERTY(EditAnywhere)
	class UMaterial* testMat_;

    TSharedPtr<MeshGenerator> meshGen_;
    TSharedPtr<RoomGenerator> roomGen_;

};
