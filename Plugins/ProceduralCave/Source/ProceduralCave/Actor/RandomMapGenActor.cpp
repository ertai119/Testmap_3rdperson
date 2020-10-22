// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMapGenActor.h"
#include <DrawDebugHelpers.h>
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"
#include <MeshDescription.h>
#include <ProceduralMeshConversion.h>
#include <StaticMeshAttributes.h>
//#include <ProceduralMeshConversion.h>

#define LIVE_CELL (0)
#define DEAD_CELL (1)

template<typename T>
static void InitArray2D(T& array, int32 height, int32 width, int32 initValue = 0)
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
}

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
				roomGen_->InitializeMap(Height, Width, SpawnProbability);
				
                for (int32 i = 0; i < SimulateCount; i++)
                {
					roomGen_->SimulateMap(BirthCount, DeathCount);
                }

				roomGen_->ProcessMap(WallThreshold, RoomThreshold, WallThresholdAfterProcessed, RoomThresholdAfterProcessed, PassageSize);

				auto fnMakeBorder = [this](const TArray<Coord>& tiles)
				{
					TArray<TArray<int32>> borderMap;
					InitArray2D(borderMap, Height + BorderSize * 2, Width + BorderSize * 2, LIVE_CELL);

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
					//GenerateMesh(maps, FColor::Black, false);
				}                

				const auto& passageInfos = roomGen_->GetPassageInfo();
				for (const auto& passage : passageInfos)
				{
					auto maps = fnMakeBorder(passage.tiles_);
					GenerateMesh(maps, FColor::Red, true);
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

void ARandomMapGenActor::GenerateMesh(const TArray<TArray<int32>>& map, const FColor& color, bool useProceduralMeshComp /*= true*/)
{
    meshGen_ = NewObject<UMeshGenerator>(this, TEXT("mesh"));
	meshGen_->InitData(map, 0.f, SquareSize);
    meshGen_->CalculateTriangle(WallHeight, true);

	if (proceduralMeshComp_ && useProceduralMeshComp)
    {
        TArray<FColor> colors;
		TArray<FVector2D> uv;
        for (int32 i = 0; i < meshGen_->vertices_.Num(); i++)
        {
            colors.Add(color);
			uv.Add(FVector2D(0.5f, 0.5f));
        }
        int32 newSectionIndex = proceduralMeshComp_->GetNumSections();
        proceduralMeshComp_->CreateMeshSection(newSectionIndex, meshGen_->vertices_, meshGen_->triangles_, {}, {}, uv, {}, {}, colors, {}, true);

		CreateToStaticMesh(newSectionIndex);
    }
    else
    {
		for (auto& position : meshGen_->vertices_)
		{
			position += GetActorLocation();
		}
		
        DrawDebugMesh(GetWorld(), meshGen_->vertices_, meshGen_->triangles_, color, true, 10.f);
    }

    //CreateFloor();
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

void ARandomMapGenActor::CreateToStaticMesh(int32 sectionIndex)
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
		FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
		SrcModel.BuildSettings.bRecomputeNormals = false;
		SrcModel.BuildSettings.bRecomputeTangents = false;
		SrcModel.BuildSettings.bRemoveDegenerates = false;
		SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
		SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
		SrcModel.BuildSettings.bGenerateLightmapUVs = true;
		SrcModel.BuildSettings.SrcLightmapIndex = 0;
		SrcModel.BuildSettings.DstLightmapIndex = 1;
		StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
		StaticMesh->CommitMeshDescription(0);

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

		//Set the Imported version before calling the build
		StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

		// Build mesh from source
		StaticMesh->Build(false);
		StaticMesh->PostEditChange();

		FString meshCompName = FString::Format(TEXT("staticmeshcomp_from_proceduralmesh_{0}"), { sectionIndex });

        UStaticMeshComponent* staticMeshComp = NewObject<UStaticMeshComponent>(this, *meshCompName);
        staticMeshComp->SetStaticMesh(StaticMesh);
        if (TestMat)
        {
			StaticMesh->SetMaterial(0, TestMat);
        }
        staticMeshComp->RegisterComponent();
        staticMeshComp->SetWorldLocation(GetActorLocation());


        spawnedStaticMeshComps_.Add(sectionIndex, staticMeshComp);

        proceduralMeshComp_->ClearMeshSection(sectionIndex);

            //proceduralMeshComp_->;
		// Notify asset registry of new asset
		//FAssetRegistryModule::AssetCreated(StaticMesh);
	}
}

void ARandomMapGenActor::InitializeMap()
{
}

void ARandomMapGenActor::DeInitializeMap()
{
    if (proceduralMeshComp_)
    {
        //proceduralMeshComp_->DestroyComponent();
        proceduralMeshComp_->ClearAllMeshSections();
    }

    for (auto& elem : spawnedStaticMeshComps_)
    {
        elem.Value->DestroyComponent();
    }

    FlushPersistentDebugLines(GetWorld());
}
