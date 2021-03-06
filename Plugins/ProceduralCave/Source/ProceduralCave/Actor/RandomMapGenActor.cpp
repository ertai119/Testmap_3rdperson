﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMapGenActor.h"
#include <DrawDebugHelpers.h>
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include <MeshDescription.h>
#include <ProceduralMeshConversion.h>
#include <StaticMeshAttributes.h>
#include "../../ApexDestruction/Source/ApexDestruction/Public/DestructibleActor.h"
#include "../../ApexDestruction/Source/ApexDestruction/Public/DestructibleMesh.h"
#include "../../ApexDestruction/Source/ApexDestruction/Public/DestructibleComponent.h"
#include "../../ApexDestruction/Source/ApexDestruction/Public/DestructibleFractureSettings.h"
#include "PhysXPublic.h"
#include "../../ApexDestruction/Source/ApexDestruction/Public/ApexDestructibleAssetImport.h"
#include <Engine/StaticMesh.h>
#include <PhysicsEngine/BodySetup.h>
#include <Materials/Material.h>

#define LIVE_CELL (0)
#define DEAD_CELL (1)

// Sets default values
ARandomMapGenActor::ARandomMapGenActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    proceduralMeshComp_ = CreateDefaultSubobject<UProceduralMeshComponent>("ProceduralMeshComponent");
}

// Called when the game starts or when spawned
void ARandomMapGenActor::BeginPlay()
{
	Super::BeginPlay();	
}

bool ARandomMapGenActor::BuildDestructibleMeshFromFractureSettings(UDestructibleMesh& DestructibleMesh, FSkeletalMeshImportData* OutData)
{
	bool Success = false;

#if WITH_APEX
	apex::DestructibleAsset* NewApexDestructibleAsset = NULL;

#if WITH_EDITORONLY_DATA
	if (DestructibleMesh.FractureSettings != NULL)
	{
		TArray<UMaterialInterface*> OverrideMaterials;
		OverrideMaterials.SetNumUninitialized(DestructibleMesh.Materials.Num());	//save old materials
		for (int32 MaterialIndex = 0; MaterialIndex < DestructibleMesh.Materials.Num(); ++MaterialIndex)
		{
			OverrideMaterials[MaterialIndex] = DestructibleMesh.Materials[MaterialIndex].MaterialInterface;
		}

		int32 MaterialCount = DestructibleMesh.FractureSettings->Materials.Num();

		DestructibleMesh.Materials.SetNum(MaterialCount);

		for (int32 MaterialIndex = 0; MaterialIndex < DestructibleMesh.Materials.Num(); ++MaterialIndex)
		{
			if (MaterialIndex < OverrideMaterials.Num() && OverrideMaterials[MaterialIndex])//if user has overridden materials use it
			{
				DestructibleMesh.Materials[MaterialIndex].MaterialInterface = OverrideMaterials[MaterialIndex];
				DestructibleMesh.Materials[MaterialIndex].ImportedMaterialSlotName = OverrideMaterials[MaterialIndex]->GetFName();
				DestructibleMesh.Materials[MaterialIndex].MaterialSlotName = OverrideMaterials[MaterialIndex]->GetFName();
			}
			else
			{
				DestructibleMesh.Materials[MaterialIndex].MaterialInterface = DestructibleMesh.FractureSettings->Materials[MaterialIndex];
				if (DestructibleMesh.FractureSettings->Materials[MaterialIndex])
				{
					DestructibleMesh.Materials[MaterialIndex].ImportedMaterialSlotName = DestructibleMesh.FractureSettings->Materials[MaterialIndex]->GetFName();
					DestructibleMesh.Materials[MaterialIndex].MaterialSlotName = DestructibleMesh.FractureSettings->Materials[MaterialIndex]->GetFName();
				}
			}
		}

		apex::DestructibleAssetCookingDesc DestructibleAssetCookingDesc;
		DestructibleMesh.FractureSettings->BuildDestructibleAssetCookingDesc(DestructibleAssetCookingDesc);
		NewApexDestructibleAsset = DestructibleMesh.FractureSettings->CreateApexDestructibleAsset(DestructibleAssetCookingDesc);
	}
#endif	// WITH_EDITORONLY_DATA

#if	WITH_EDITOR
	if (NewApexDestructibleAsset != NULL)
	{
		Success = SetApexDestructibleAsset(DestructibleMesh, *NewApexDestructibleAsset, OutData, EDestructibleImportOptions::PreserveSettings);
	}
#endif

#endif // WITH_APEX

	return Success;
}

#if WITH_EDITOR
void ARandomMapGenActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    if (PropertyChangedEvent.MemberProperty)
    {
        const FName MemberPropName = PropertyChangedEvent.MemberProperty->GetFName();

        if (MemberPropName == GET_MEMBER_NAME_CHECKED(ARandomMapGenActor, Generate))
        {
            if (Generate)
            {
				roomGen_ = NewObject<URoomGenerator>(this, TEXT("room"));
				roomGen_->InitializeMap(Height, Width, RandomSeed, SpawnProbability);
				
                for (int32 i = 0; i < SimulateCount; i++)
                {
					roomGen_->SimulateMap(BirthCount, DeathCount);
                }

				roomGen_->ProcessMap(WallThreshold, RoomThreshold, WallThresholdAfterProcessed, RoomThresholdAfterProcessed, PassageSize);

				auto fnMakeBorder = [this](const TArray<Coord>& tiles)
				{
					auto fnInitArray = [](TArray<TArray<int32>>& array, int32 height, int32 width, int32 initValue = 0)
					{
						array.Empty();
						array.SetNum(height);
						for (int32 i = 0; i < array.Num(); i++)
						{
							array[i].SetNum(width);
							for (auto& value : array[i])
							{
								value = initValue;
							}
						}
					};

					TArray<TArray<int32>> borderMap;
					fnInitArray(borderMap, Height + BorderSize * 2, Width + BorderSize * 2, LIVE_CELL);

					for (const auto& tile : tiles)
					{
						borderMap[tile.tileY_ + BorderSize][tile.tileX_ + BorderSize] = DEAD_CELL;
					}
					return borderMap;
				};                

				TArray<TArray<Coord>> allWalls = roomGen_->GetRegions(DEAD_CELL);
				for (const auto& wall : allWalls)
				{
					auto maps = fnMakeBorder(wall);
					GenerateMesh(maps, FColor::Black, false, false);
				}                

				TArray<int32> targetMeshSections;
				const auto& passageInfos = roomGen_->GetPassageInfo();
				for (const auto& passage : passageInfos)
				{
					auto maps = fnMakeBorder(passage.tiles_);
					int32 ret = GenerateMesh(maps, FColor::Red, true, true);
					if (ret != INDEX_NONE)
					{
						targetMeshSections.Add(ret);
					}
				}

				CreateFloor();

				for (int32 sectionIndex : targetMeshSections)
				{
					ConvertToDestructibleMeshFromStaticMesh(sectionIndex);
				}
            }
            else
            {
                DeInitializeMap();
            }
        }

        if (MemberPropName == GET_MEMBER_NAME_CHECKED(ARandomMapGenActor, Simulate))
        {
            if (Simulate)
            {
                //SimulateMap();
                //DrawCells();
            }
        }
    }

	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

// Called every frame
void ARandomMapGenActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int32 ARandomMapGenActor::GenerateMesh(const TArray<TArray<int32>>& map, const FColor& color, bool covertStaticMesh, bool ShowDebugMesh)
{
	int32 retMeshSection = INDEX_NONE;

    meshGen_ = NewObject<UMeshGenerator>(this, TEXT("mesh"));
	meshGen_->InitData(map, 0.f, SquareSize);
    meshGen_->CalculateTriangle(WallHeight, true);

	if (proceduralMeshComp_)
    {
        TArray<FColor> colors;
		TArray<FVector2D> uv;
        for (int32 i = 0; i < meshGen_->vertices_.Num(); i++)
        {
            colors.Add(color);
			uv.Add(FVector2D(0.5f, 0.5f));
        }
        int32 newSectionIndex = proceduralMeshComp_->GetNumSections();
		retMeshSection = newSectionIndex;
        proceduralMeshComp_->CreateMeshSection(newSectionIndex, meshGen_->vertices_, meshGen_->triangles_, {}, {}, uv, {}, {}, colors, {}, true);

		if (covertStaticMesh)
		{
			ConvertToStaticMeshFromProceduralMeshComp(newSectionIndex);
		}
    }

	if (ShowDebugMesh)
	{
		for (auto& position : meshGen_->vertices_)
		{
			position += GetActorLocation();

			DrawDebugPoint(GetWorld(), position, 10.f, FColor::Red);
		}
		//DrawDebugMesh(GetWorld(), meshGen_->vertices_, meshGen_->triangles_, color, true, 10.f);		
	}

	return retMeshSection;
}

void ARandomMapGenActor::CreateFloor()
{
	TArray<FVector> floorVertices;
	TArray<int32> floorTriangles;

    FVector bottomLeft = FVector(-SquareSize * Width * 0.5f, -SquareSize * Height * 0.5f, -WallHeight);
    FVector topLeft = FVector(-SquareSize * Width * 0.5f, SquareSize * Height * 0.5f, -WallHeight);
    FVector topRight = FVector(SquareSize * Width * 0.5f, SquareSize * Height * 0.5f, -WallHeight);
    FVector bottomRight = FVector(SquareSize * Width * 0.5f, -SquareSize * Height * 0.5f, -WallHeight);

    floorVertices.Add(bottomLeft);
    floorVertices.Add(topLeft);
    floorVertices.Add(topRight);
    floorVertices.Add(bottomRight);

    floorTriangles.Add(0);
    floorTriangles.Add(1);
    floorTriangles.Add(2);

    floorTriangles.Add(0);
    floorTriangles.Add(2);
    floorTriangles.Add(3);

	TArray<FColor> colors;
	for (int32 i = 0; i < floorVertices.Num(); i++)
	{
		colors.Add(FColor::Purple);
	}
	int32 newSectionIndex = proceduralMeshComp_->GetNumSections();
	proceduralMeshComp_->CreateMeshSection(newSectionIndex, floorVertices, floorTriangles, {}, {}, {}, {}, {}, colors, {}, true);
}

FVector ARandomMapGenActor::CoordToWorldPosition(const Coord& coord) const
{
    /*FVector pos(-mapWidth * 0.5f + x * squareSize + squareSize * 0.5f
        , -mapHeight * 0.5f + y * squareSize + squareSize * 0.5f
        , 0.f);*/

    float mapWidth = SquareSize * Width;
    float mapHeight = SquareSize * Height;
    return FVector(-mapWidth * 0.5f + coord.tileX_ * SquareSize + SquareSize * 0.5f
        , -mapHeight * 0.5f + coord.tileY_ * SquareSize + SquareSize * 0.5f
        , 0.f) + GetActorLocation();
}

static TMap<UMaterialInterface*, FPolygonGroupID> BuildMaterialMap(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription, int32 wantSectionIndex)
{
	TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials;
	//const int32 NumSections = ProcMeshComp->GetNumSections();
	UniqueMaterials.Reserve(1);

	FStaticMeshAttributes AttributeGetter(MeshDescription);
	TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
	//for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection =
			ProcMeshComp->GetProcMeshSection(wantSectionIndex);
		UMaterialInterface* Material = ProcMeshComp->GetMaterial(wantSectionIndex);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}

		if (!UniqueMaterials.Contains(Material))
		{
			FPolygonGroupID NewPolygonGroup = MeshDescription.CreatePolygonGroup();
			UniqueMaterials.Add(Material, NewPolygonGroup);
			PolygonGroupNames[NewPolygonGroup] = Material->GetFName();
		}
	}
	return UniqueMaterials;
}

static FMeshDescription BuildMeshDescription(UProceduralMeshComponent* ProcMeshComp, int32 wantSectionIndex)
{
	FMeshDescription MeshDescription;

	FStaticMeshAttributes AttributeGetter(MeshDescription);
	AttributeGetter.Register();

	TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
	TVertexAttributesRef<FVector> VertexPositions = AttributeGetter.GetVertexPositions();
	TVertexInstanceAttributesRef<FVector> Tangents = AttributeGetter.GetVertexInstanceTangents();
	TVertexInstanceAttributesRef<float> BinormalSigns = AttributeGetter.GetVertexInstanceBinormalSigns();
	TVertexInstanceAttributesRef<FVector> Normals = AttributeGetter.GetVertexInstanceNormals();
	TVertexInstanceAttributesRef<FVector4> Colors = AttributeGetter.GetVertexInstanceColors();
	TVertexInstanceAttributesRef<FVector2D> UVs = AttributeGetter.GetVertexInstanceUVs();

	// Materials to apply to new mesh
	const int32 NumSections = ProcMeshComp->GetNumSections();
	int32 VertexCount = 0;
	int32 VertexInstanceCount = 0;
	int32 PolygonCount = 0;

	TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials = BuildMaterialMap(ProcMeshComp, MeshDescription, wantSectionIndex);
	TArray<FPolygonGroupID> PolygonGroupForSection;
	PolygonGroupForSection.Reserve(1);

	// Calculate the totals for each ProcMesh element type
	//for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection =
			ProcMeshComp->GetProcMeshSection(wantSectionIndex);
		VertexCount += ProcSection->ProcVertexBuffer.Num();
		VertexInstanceCount += ProcSection->ProcIndexBuffer.Num();
		PolygonCount += ProcSection->ProcIndexBuffer.Num() / 3;
	}
	MeshDescription.ReserveNewVertices(VertexCount);
	MeshDescription.ReserveNewVertexInstances(VertexInstanceCount);
	MeshDescription.ReserveNewPolygons(PolygonCount);
	MeshDescription.ReserveNewEdges(PolygonCount * 2);
	UVs.SetNumIndices(4);

	// Create the Polygon Groups
	//for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection =
			ProcMeshComp->GetProcMeshSection(wantSectionIndex);
		UMaterialInterface* Material = ProcMeshComp->GetMaterial(wantSectionIndex);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}

		FPolygonGroupID* PolygonGroupID = UniqueMaterials.Find(Material);
		check(PolygonGroupID != nullptr);
		PolygonGroupForSection.Add(*PolygonGroupID);
	}


	// Add Vertex and VertexInstance and polygon for each section
	//for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection =
			ProcMeshComp->GetProcMeshSection(wantSectionIndex);
		FPolygonGroupID PolygonGroupID = PolygonGroupForSection[0];
		// Create the vertex
		int32 NumVertex = ProcSection->ProcVertexBuffer.Num();
		TMap<int32, FVertexID> VertexIndexToVertexID;
		VertexIndexToVertexID.Reserve(NumVertex);
		for (int32 VertexIndex = 0; VertexIndex < NumVertex; ++VertexIndex)
		{
			FProcMeshVertex& Vert = ProcSection->ProcVertexBuffer[VertexIndex];
			const FVertexID VertexID = MeshDescription.CreateVertex();
			VertexPositions[VertexID] = Vert.Position;
			VertexIndexToVertexID.Add(VertexIndex, VertexID);
		}
		// Create the VertexInstance
		int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
		int32 NumTri = NumIndices / 3;
		TMap<int32, FVertexInstanceID> IndiceIndexToVertexInstanceID;
		IndiceIndexToVertexInstanceID.Reserve(NumVertex);
		for (int32 IndiceIndex = 0; IndiceIndex < NumIndices; IndiceIndex++)
		{
			const int32 VertexIndex = ProcSection->ProcIndexBuffer[IndiceIndex];
			const FVertexID VertexID = VertexIndexToVertexID[VertexIndex];
			const FVertexInstanceID VertexInstanceID =
				MeshDescription.CreateVertexInstance(VertexID);
			IndiceIndexToVertexInstanceID.Add(IndiceIndex, VertexInstanceID);

			FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[VertexIndex];

			Tangents[VertexInstanceID] = ProcVertex.Tangent.TangentX;
			Normals[VertexInstanceID] = ProcVertex.Normal;
			BinormalSigns[VertexInstanceID] =
				ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f;

			Colors[VertexInstanceID] = FLinearColor(ProcVertex.Color);

			UVs.Set(VertexInstanceID, 0, ProcVertex.UV0);
			UVs.Set(VertexInstanceID, 1, ProcVertex.UV1);
			UVs.Set(VertexInstanceID, 2, ProcVertex.UV2);
			UVs.Set(VertexInstanceID, 3, ProcVertex.UV3);
		}

		// Create the polygons for this section
		for (int32 TriIdx = 0; TriIdx < NumTri; TriIdx++)
		{
			FVertexID VertexIndexes[3];
			TArray<FVertexInstanceID> VertexInstanceIDs;
			VertexInstanceIDs.SetNum(3);

			for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
			{
				const int32 IndiceIndex = (TriIdx * 3) + CornerIndex;
				const int32 VertexIndex = ProcSection->ProcIndexBuffer[IndiceIndex];
				VertexIndexes[CornerIndex] = VertexIndexToVertexID[VertexIndex];
				VertexInstanceIDs[CornerIndex] =
					IndiceIndexToVertexInstanceID[IndiceIndex];
			}

			// Insert a polygon into the mesh
			MeshDescription.CreatePolygon(PolygonGroupID, VertexInstanceIDs);
		}
	}
	return MeshDescription;
}

void ARandomMapGenActor::ConvertToStaticMeshFromProceduralMeshComp(int32 sectionIndex)
{
    if (proceduralMeshComp_ == nullptr)
    {
        return;
    }

	FMeshDescription MeshDescription = BuildMeshDescription(proceduralMeshComp_, sectionIndex);

	// If we got some valid data.
	if (MeshDescription.Polygons().Num() > 0)
	{
		// Then find/create it.
		//UPackage* Package = CreatePackage(NULL, *UserPackageName);
		//check(Package);

        FString meshName = FString::Format(TEXT("staticmesh_from_proceduralmesh_{0}"), { sectionIndex });
		// Create StaticMesh object
		UStaticMesh* StaticMesh = NewObject<UStaticMesh>(this, *meshName);
		StaticMesh->InitResources();

		StaticMesh->LightingGuid = FGuid::NewGuid();

		// Add source to new StaticMesh
#if WITH_EDITOR
		FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
		SrcModel.BuildSettings.bRecomputeNormals = true;
		SrcModel.BuildSettings.bRecomputeTangents = false;
		SrcModel.BuildSettings.bRemoveDegenerates = false;
		SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
		SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
		SrcModel.BuildSettings.bGenerateLightmapUVs = true;
		SrcModel.BuildSettings.SrcLightmapIndex = 0;
		SrcModel.BuildSettings.DstLightmapIndex = 1;
		StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
		StaticMesh->CommitMeshDescription(0);
#endif

		//// SIMPLE COLLISION
		if (!proceduralMeshComp_->bUseComplexAsSimpleCollision)
		{
			StaticMesh->CreateBodySetup();
			UBodySetup* NewBodySetup = StaticMesh->BodySetup;
			NewBodySetup->BodySetupGuid = FGuid::NewGuid();
			NewBodySetup->AggGeom.ConvexElems = proceduralMeshComp_->ProcMeshBodySetup->AggGeom.ConvexElems;
			NewBodySetup->bGenerateMirroredCollision = false;
			NewBodySetup->bDoubleSidedGeometry = true;
			NewBodySetup->CollisionTraceFlag = CTF_UseDefault;
			NewBodySetup->CreatePhysicsMeshes();
			StaticMesh->CreateNavCollision(true);
		}

		//// MATERIALS
		TSet<UMaterialInterface*> UniqueMaterials;
		//const int32 NumSections = proceduralMeshComp_->GetNumSections();
		//for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
		{
			FProcMeshSection* ProcSection =
                proceduralMeshComp_->GetProcMeshSection(sectionIndex);
			UMaterialInterface* Material = proceduralMeshComp_->GetMaterial(sectionIndex);
			UniqueMaterials.Add(Material);
		}
		// Copy materials to new mesh
		for (auto* Material : UniqueMaterials)
		{
			StaticMesh->StaticMaterials.Add(FStaticMaterial(Material));
		}

#if WITH_EDITORONLY_DATA
		//Set the Imported version before calling the build
		StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;
#endif 
#if WITH_EDITOR
		// Build mesh from source
		StaticMesh->Build(false);
		StaticMesh->PostEditChange();
#endif
		FString meshCompName = FString::Format(TEXT("staticmeshcomp_from_proceduralmesh_{0}"), { sectionIndex });

        UStaticMeshComponent* staticMeshComp = NewObject<UStaticMeshComponent>(this, *meshCompName);
        staticMeshComp->SetStaticMesh(StaticMesh);
#if WITH_EDITOR
        if (TestMat)
        {
			StaticMesh->SetMaterial(0, TestMat);
        }
#endif
        staticMeshComp->RegisterComponent();
        staticMeshComp->SetWorldLocation(GetActorLocation());

        spawnedStaticMeshComps_.Add(sectionIndex, staticMeshComp);

		//proceduralMeshComp_->SetMeshSectionVisible(sectionIndex, false);
        proceduralMeshComp_->ClearMeshSection(sectionIndex);
	}
}

void ARandomMapGenActor::ConvertToDestructibleMeshFromStaticMesh(int32 sectionIndex)
{
	auto it = spawnedStaticMeshComps_.Find(sectionIndex);
	if (it == nullptr)
	{
		return;
	}

	auto destructibleActor = GetWorld()->SpawnActor<ADestructibleActor>(ADestructibleActor::StaticClass(), GetActorTransform());
	if (destructibleActor == nullptr)
	{
		return;
	}

	(*it)->SetVisibility(false);

	spawnedDestructibleActors_.Add(destructibleActor);

	UStaticMesh* staticMesh = (*it)->GetStaticMesh();
	if (staticMesh)
	{
		UDestructibleMesh* DestructibleMesh = NewObject<UDestructibleMesh>(this, *FString::Format(TEXT("dest_mesh{0}"), { sectionIndex }));
		DestructibleMesh->BuildFromStaticMesh(*staticMesh);

		DestructibleMesh->DefaultDestructibleParameters.DamageParameters.bEnableImpactDamage = true;
		DestructibleMesh->DefaultDestructibleParameters.DamageParameters.ImpactDamage = 1.f;

#if WITH_APEX
		if (DestructibleMesh != NULL)
		{
			TComponentReregisterContext<USkinnedMeshComponent> ReregisterContext;

			DestructibleMesh->ReleaseResources();
			DestructibleMesh->ReleaseResourcesFence.Wait();

			if (DestructibleMesh->SourceStaticMesh)
			{
				DestructibleMesh->BuildFractureSettingsFromStaticMesh(DestructibleMesh->SourceStaticMesh);
			}
			else if (DestructibleMesh->ApexDestructibleAsset != NULL)
			{
				DestructibleMesh = ImportDestructibleMeshFromApexDestructibleAsset(DestructibleMesh->GetOuter(), *DestructibleMesh->ApexDestructibleAsset, DestructibleMesh->GetFName(), DestructibleMesh->GetFlags(), NULL,
					EDestructibleImportOptions::PreserveSettings);
			}

			DestructibleMesh->FractureSettings->CreateVoronoiSitesInRootMesh();
			DestructibleMesh->FractureSettings->VoronoiSplitMesh();


			BuildDestructibleMeshFromFractureSettings(*DestructibleMesh, NULL);
		}
#endif // WITH_APEX

		destructibleActor->GetDestructibleComponent()->SetDestructibleMesh(DestructibleMesh);
	}
}

void ARandomMapGenActor::InitializeMap()
{
}

void ARandomMapGenActor::DeInitializeMap()
{
    if (proceduralMeshComp_)
    {
        proceduralMeshComp_->ClearAllMeshSections();
    }

    for (auto& elem : spawnedStaticMeshComps_)
    {
		if (elem.Value)
		{
			elem.Value->DestroyComponent();
		}
    }
	spawnedStaticMeshComps_.Empty();

	for (auto& elem : spawnedDestructibleActors_)
	{
		if (elem.IsValid())
		{
			GetWorld()->DestroyActor(elem.Get());
		}
	}
	spawnedDestructibleActors_.Empty();

    FlushPersistentDebugLines(GetWorld());
}
