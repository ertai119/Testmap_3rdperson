// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RandomMapGenActor.generated.h"

USTRUCT()
struct FCellInfo
{
    GENERATED_BODY()

    FVector2D pos;
    bool dead;

    UPROPERTY()
    UStaticMeshComponent* meshComp;
};

class UNodeBase
{
public:
    FVector position_;
    int32 vertexIndex_ = INDEX_NONE;

    virtual ~UNodeBase() {}

    explicit UNodeBase(FVector pos)
    {
        position_ = pos;
    }
};

using NodePtr = TSharedPtr<UNodeBase>;

class UControlNode : public UNodeBase
{

public:
    bool active_ = false;
    NodePtr above_, right_;

    virtual ~UControlNode() {}

    UControlNode(FVector pos, bool active, float squareSize)
        : UNodeBase(pos)
    {
        active_ = active;
        above_ = MakeShared<UNodeBase>(pos + FVector(0.f, 1.f, 0.f) * squareSize * 0.5f);
        right_ = MakeShared<UNodeBase>(pos + FVector(1.f, 0.f, 0.f) * squareSize * 0.5f);
    }
};
using ControlNodePtr = TSharedPtr<UControlNode>;

class Triangle 
{
public:
    int32 vertexIndexA_;
    int32 vertexIndexB_;
    int32 vertexIndexC_;
    TArray<int32> vertices_;

    Triangle(int32 a, int32 b, int32 c)
    {
        vertexIndexA_ = a;
        vertexIndexB_ = b;
        vertexIndexC_ = c;

        vertices_.SetNum(3);
        vertices_[0] = a;
        vertices_[1] = b;
        vertices_[2] = c;
    }

    bool Contains(int32 vertexIndex) const
    {
        return vertexIndexA_ == vertexIndex || vertexIndexB_ == vertexIndex || vertexIndexC_ == vertexIndex;
    }
};
using TrianglePtr = TSharedPtr<Triangle>;

class Coord
{
public:
    int32 tileX_;
    int32 tileY_;

    Coord() = default;
    Coord(int32 x, int32 y)
    {
        tileX_ = x;
        tileY_ = y;
    }

    bool operator==(const Coord& other) const
    {
        return tileX_ == other.tileX_ && tileY_ == other.tileY_;
    }
    bool operator!=(const Coord& other) const
    {
        return !(*this == other);
    }
};

using RoomPtr = TSharedPtr<class Room>;
class Room
{
public:
    TArray<Coord> tiles_;
    TArray<Coord> edgeTiles_;
    TArray<RoomPtr> connectedRooms_;
    int32 roomSize_ = 0;
    bool isAccessibleFromMainRoom_ = false;
    bool isMainRoom_ = false;

    Room() = default;
    Room(const TArray<Coord>& tiles, const TArray<TArray<int32>>& map)
    {
        tiles_ = tiles;
        roomSize_ = tiles.Num();

        for (const auto& tile : tiles_)
        {
            for (int32 x = tile.tileX_ - 1; x <= tile.tileX_ + 1; x++)
            {
                for (int32 y = tile.tileY_ - 1; y <= tile.tileY_ + 1; y++)
                {
                    if (x == tile.tileX_ || y == tile.tileY_)
                    {
                        if (map.IsValidIndex(y) == false)
                        {
                            continue;
                        }

                        if (map[y].IsValidIndex(x) == false)
                        {
                            continue;
                        }

                        if (map[y][x] == 1)
                        {
                            edgeTiles_.Add(tile);
                        }
                    }
                }
            }
        }
    }

    static void ConnectRooms(RoomPtr roomA, RoomPtr roomB)
    {
        if (roomA->isAccessibleFromMainRoom_)
        {
            roomB->SetAccessibleFromMainRoom();
        }
        else if (roomB->isAccessibleFromMainRoom_)
        {
            roomA->SetAccessibleFromMainRoom();
        }

        roomA->connectedRooms_.Add(roomB);
        roomB->connectedRooms_.Add(roomA);
    }

    bool IsConnected(RoomPtr room)
    {
        return connectedRooms_.Contains(room);         
    }

    void SetAccessibleFromMainRoom()
    {
        if (isAccessibleFromMainRoom_ == false)
        {
            isAccessibleFromMainRoom_ = true;
            for (auto& room : connectedRooms_)
            {
                room->SetAccessibleFromMainRoom();
            }
        }
    }
};

class USquare
{
public:
    ControlNodePtr topLeft_, topRight_, bottomRight_, bottomLeft_;
    NodePtr centerTop_, centerRight_, centerBottom_, centerLeft_;
    int32 configuration_ = 0;

    USquare(ControlNodePtr topLeft, ControlNodePtr topRight, ControlNodePtr bottomRight, ControlNodePtr bottomLeft)
    {
        topLeft_ = topLeft;
        topRight_ = topRight;
        bottomRight_ = bottomRight;
        bottomLeft_ = bottomLeft;

        centerTop_ = topLeft->right_;
        centerRight_ = bottomRight->above_;
        centerBottom_ = bottomLeft->right_;
        centerLeft_ = bottomLeft->above_;

        if (topLeft->active_)
        {
            configuration_ += 8;
        }
        if (topRight->active_)
        {
            configuration_ += 4;
        }
        if (bottomRight->active_)
        {
            configuration_ += 2;
        }
        if (bottomLeft->active_)
        {
            configuration_ += 1;
        }
    }
};
using SquarePtr = TSharedPtr<USquare>;

class USquareGrid
{
public:
    TArray<TArray<SquarePtr>> squares_;

    USquareGrid(TArray<TArray<int32>> map, float squareSize)
    {
        int32 nodeCountY = map.Num();
        int32 nodeCountX = map[0].Num();
        float mapWidth = nodeCountX * squareSize;
        float mapHeight = nodeCountY * squareSize;

        TArray<TArray<ControlNodePtr>> controlNodes;
        controlNodes.SetNum(nodeCountY);
        for (int32 i = 0; i < nodeCountY; i++)
        {
            controlNodes[i].SetNum(nodeCountX);
        }

        for (int32 y = 0; y < nodeCountY; y++)
        {
            for (int32 x = 0; x < nodeCountX; x++)
            {
                FVector pos(-mapWidth * 0.5f + x * squareSize + squareSize * 0.5f
                    , -mapHeight * 0.5f + y * squareSize + squareSize * 0.5f
                    , 0.f);

                controlNodes[y][x] = MakeShared<UControlNode>(pos, map[y][x] == 1, squareSize);
            }
        }

        squares_.SetNum(nodeCountY - 1);
        for (int32 i = 0; i < nodeCountY - 1; i++)
        {
            squares_[i].SetNum(nodeCountX - 1);
        }

        for (int32 y = 0; y < nodeCountY - 1; y++)
        {            
            for (int32 x = 0; x < nodeCountX - 1; x++)
            {
                //USquare(ControlNodePtr topLeft, ControlNodePtr topRight, ControlNodePtr bottomRight, ControlNodePtr bottomLeft)
                squares_[y][x] = MakeShared<USquare>(controlNodes[y + 1][x]
                    , controlNodes[y + 1][x + 1]
                    , controlNodes[y][x + 1]
                    , controlNodes[y][x]);
            }
        }
    }
};

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

    void SimulateMap();

    void DrawCells();

    int32 CountAliveNeighbours(int32 x, int32 y);


    void GenerateMesh(const TArray<TArray<int32>>& map);
    void CreateWallMesh();
    void DebugDrawMesh();
    void ProcessMap();
    void CreateFloor();
    void CreateBorder(int32 borderSize);
    void ConnectedClosestRooms(const TArray<RoomPtr>& allRooms, bool forceAccessbilityFromMainRoom = false);
    void CreatePassage(RoomPtr roomA, RoomPtr roomB, const Coord& tileA, const Coord& tileB);
    void DrawCircle(const Coord& tile, int32 r);
    TArray<Coord> GetLine(Coord from, Coord to) const;
    void TriangulateSquare(SquarePtr square);
    void MeshFromPoints(const TArray<NodePtr>& points);
    void AssignVertices(const TArray<NodePtr>& points);
    void CreateTriangle(NodePtr a, NodePtr b, NodePtr c);
    void AddTriangleToMap(int32 vertexKey, TrianglePtr triangle);
    bool IsOutlineEdge(int32 vertexA, int32 vertexB);
    int32 GetConnectedOutlineVertex(int32 vertexIndex);
    void CalculateMeshOutlines();
    void FollowOutline(int32 vertexIndex, int32 outlineIndex);
    TArray<TArray<Coord>> GetRegions(int32 tileType) const;
    TArray<Coord> GetRegionTiles(int32 startX, int32 startY) const;
    bool IsInMap(int32 x, int32 y) const;
    FVector CoordToWorldPosition(const Coord& coord) const;

    UPROPERTY(EditAnywhere)
    UStaticMesh* FloorMesh;

    UPROPERTY(EditAnywhere)
    UStaticMesh* WallMesh;

    UPROPERTY(EditAnywhere)
    bool Generate;

    UPROPERTY(EditAnywhere)
    bool Simulate;

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

    UPROPERTY()
    TArray<UStaticMeshComponent*> meshs_;
    UPROPERTY()
    TArray<UStaticMeshComponent*> borderMeshs_;

    UPROPERTY(EditAnywhere)
    class UProceduralMeshComponent* proceduralMeshComp_;

    TArray<TArray<int32>> maps_;

    TSharedPtr<USquareGrid> squareGrid_;
    TArray<FVector> vertices_;
    TArray<int32> triangles_;
    TMap<int32, TArray<TrianglePtr>> triangleMaps_;
    TArray<TArray<int32>> outlines_;
    TSet<int32> checkedVertices_;
};
