// Fill out your copyright notice in the Description page of Project Settings.


#include "RandomMapGenActor.h"
#include <DrawDebugHelpers.h>
#include "../Plugins/Runtime/ProceduralMeshComponent/Source/ProceduralMeshComponent/Public/ProceduralMeshComponent.h"

#define LIVE_CELL (0)
#define DEAD_CELL (1)

template<typename T>
static void InitArray2D(T& array, int32 height, int32 width)
{
    array.Empty();
    array.SetNum(height);
    for (int32 i = 0; i < array.Num(); i++)
    {
        array[i].SetNum(width);
        for (auto& value : array[i])
        {
            value = 0;
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
                InitializeMap();
                //DrawCells();
                
                for (int32 i = 0; i < 2; i++)
                {
                    SimulateMap();
                }
                
                ProcessMap();

                CreateBorder(BorderSize);
                CreateFloor();

                TArray<TArray<int32>> borderMap;
                InitArray2D(borderMap, Height + BorderSize * 2, Width + BorderSize * 2);
                
                for (int32 x = 0; x < borderMap[0].Num(); x++)
                {
                    for (int32 y = 0; y < borderMap.Num(); y++)
                    {
                        if (x >= BorderSize && x < Width + BorderSize && y >= BorderSize && y < Height + BorderSize)
                        {
                            borderMap[y][x] = maps_[y- BorderSize][x- BorderSize];
                        }
                        else
                        {
                            borderMap[y][x] = DEAD_CELL;
                        }
                    }
                }

                GenerateMesh(borderMap);
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
                SimulateMap();
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

void ARandomMapGenActor::CreateFloor()
{

}

void ARandomMapGenActor::CreateBorder(int32 borderSize)
{

}

int32 ARandomMapGenActor::CountAliveNeighbours(int32 x, int32 y)
{
    int32 count = 0;
    for (int32 i = -1; i < 2; i++)
    {
        for (int32 j = -1; j < 2; j++)
        {
            int32 neighbour_x = x + i;
            int32 neighbour_y = y + j;

            if (i == 0 && j == 0)
            {
            }
            else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= Width || neighbour_y >= Height)
            {
                count += 1;
            }
            else if (maps_[neighbour_y][neighbour_x] == 1)
            {
                count += 1;
            }
        }
    }
    return count;
}

void ARandomMapGenActor::GenerateMesh(const TArray<TArray<int32>>& map)
{
    squareGrid_ = MakeShared<USquareGrid>(map, SquareSize);

    for (int32 y = 0; y < squareGrid_->squares_.Num(); y++)
    {
        for (int32 x = 0; x < squareGrid_->squares_[0].Num(); x++)
        {
            TriangulateSquare(squareGrid_->squares_[y][x]);
        }
    }

    //int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals, const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision
    if (proceduralMeshComp_)
    {
        TArray<FColor> colors;
        for (int32 i = 0; i < vertices_.Num(); i++)
        {
            colors.Add(FColor::Black);
        }
        proceduralMeshComp_->CreateMeshSection(1, vertices_, triangles_, {}, {}, {}, {}, {}, colors, {}, true);
    }
    else
    {
        DrawDebugMesh(GetWorld(), vertices_, triangles_, FColor::Black, false, 10.f);
    }

    CreateWallMesh();
}

void ARandomMapGenActor::CreateWallMesh()
{
    CalculateMeshOutlines();

    TArray<FVector> wallVertices;
    TArray<int32> wallTriangles;
    
    for (const auto& outline : outlines_)
    {
        for (int32 i = 0; i < outline.Num() - 1; i++)
        {
            int32 startIndex = wallVertices.Num();
            wallVertices.Add(vertices_[outline[i]]); // left
            wallVertices.Add(vertices_[outline[i + 1]]); // right
            wallVertices.Add(vertices_[outline[i]] - FVector::UpVector * WallHeight); // bottom left
            wallVertices.Add(vertices_[outline[i + 1]] - FVector::UpVector * WallHeight); // bottom right

            wallTriangles.Add(startIndex + 0);
            wallTriangles.Add(startIndex + 2);
            wallTriangles.Add(startIndex + 3);

            wallTriangles.Add(startIndex + 3);
            wallTriangles.Add(startIndex + 1);
            wallTriangles.Add(startIndex + 0);
        }            
    }

    TArray<FColor> colors;
    for (int32 i = 0; i < vertices_.Num(); i++)
    {
        colors.Add(FColor::Purple);
    }
    proceduralMeshComp_->CreateMeshSection(2, wallVertices, wallTriangles, {}, {}, {}, {}, {}, colors, {}, true);
}

void ARandomMapGenActor::DebugDrawMesh()
{
    if (squareGrid_.IsValid() == false)
    {
        return;
    }

    const auto& originPos = GetActorLocation();

    for (int32 y = 0; y < squareGrid_->squares_.Num(); y++)
    {
        for (int32 x = 0; x < squareGrid_->squares_[0].Num(); x++)
        {
            auto& info = squareGrid_->squares_[y][x];
            FColor color = info->topLeft_->active_ ? FColor::Green : FColor::Black;             
            DrawDebugSolidBox(GetWorld(), info->topLeft_->position_ + originPos, FVector::OneVector * 4.f, color, false, 10.f);

            color = info->topRight_->active_ ? FColor::Green : FColor::Black;
            DrawDebugSolidBox(GetWorld(), info->topRight_->position_ + originPos, FVector::OneVector * 4.f, color, false, 10.f);

            color = info->bottomRight_->active_ ? FColor::Green : FColor::Black;
            DrawDebugSolidBox(GetWorld(), info->bottomRight_->position_ + originPos, FVector::OneVector * 4.f, color, false, 10.f);

            color = info->bottomLeft_->active_ ? FColor::Green : FColor::Black;
            DrawDebugSolidBox(GetWorld(), info->bottomLeft_->position_ + originPos, FVector::OneVector * 4.f, color, false, 10.f);

            color = FColor::Silver;
            DrawDebugSolidBox(GetWorld(), info->centerTop_->position_ + originPos, FVector::OneVector * 2.f, color, false, 10.f);
            DrawDebugSolidBox(GetWorld(), info->centerRight_->position_ + originPos, FVector::OneVector * 2.f, color, false, 10.f);
            DrawDebugSolidBox(GetWorld(), info->centerBottom_->position_ + originPos, FVector::OneVector * 2.f, color, false, 10.f);
            DrawDebugSolidBox(GetWorld(), info->centerLeft_->position_ + originPos, FVector::OneVector * 2.f, color, false, 10.f);

        }
    }
}

void ARandomMapGenActor::ProcessMap()
{
    TArray<TArray<Coord>> wallRegions = GetRegions(DEAD_CELL);
    for (const auto& coordList : wallRegions)
    {
        if (coordList.Num() < WallThreshold)
        {
            for (const auto& coord : coordList)
            {
                maps_[coord.tileY_][coord.tileX_] = LIVE_CELL;
            }
        }
    }

    TArray<TArray<Coord>> roomRegions = GetRegions(LIVE_CELL);
    TArray<RoomPtr> survivingRooms;
    for (const auto& coordList : roomRegions)
    {
        if (coordList.Num() < RoomThreshold)
        {
            for (const auto& coord : coordList)
            {
                maps_[coord.tileY_][coord.tileX_] = DEAD_CELL;
            }
        }
        else
        {
            RoomPtr newRoom = MakeShared<Room>(coordList, maps_);
            survivingRooms.Add(newRoom);

            FColor color(FMath::RandRange(0, 255), FMath::RandRange(0, 255), FMath::RandRange(0, 255));
            
            /*for (const auto& tile : newRoom->edgeTiles_)
            {
                FVector pos = CoordToWorldPosition(tile);

                DrawDebugPoint(GetWorld(), pos, SquareSize * 0.3f, color, true, 10.f);
            }*/
            
            /*for (const auto& tile : newRoom->tiles_)
            {
                FVector pos = CoordToWorldPosition(tile);

                DrawDebugPoint(GetWorld(), pos, SquareSize * 0.3f, color, true, 10.f);
            }*/
        }
    }

    survivingRooms.Sort([](const auto& lhs, const auto& rhs)
    {
        return lhs->roomSize_ > rhs->roomSize_;
    });

    if (survivingRooms.Num() > 0)
    {
        survivingRooms[0]->isAccessibleFromMainRoom_ = true;
        survivingRooms[0]->isMainRoom_ = true;
    }

    ConnectedClosestRooms(survivingRooms);

    TArray<TArray<Coord>> wallRegions2nd = GetRegions(DEAD_CELL);
    for (const auto& coordList : wallRegions2nd)
    {
        if (coordList.Num() < WallThresholdAfterProcessed)
        {
            for (const auto& coord : coordList)
            {
                maps_[coord.tileY_][coord.tileX_] = LIVE_CELL;
            }
        }
    }

	TArray<TArray<Coord>> roomRegions2nd = GetRegions(LIVE_CELL);
	for (const auto& coordList : roomRegions2nd)
	{
		if (coordList.Num() < RoomThresholdAfterProcessed)
		{
			for (const auto& coord : coordList)
			{
				maps_[coord.tileY_][coord.tileX_] = DEAD_CELL;
			}
		}
	}
}

void ARandomMapGenActor::ConnectedClosestRooms(const TArray<RoomPtr>& allRooms, bool forceAccessbilityFromMainRoom /*= false*/)
{
    TArray<RoomPtr> roomListA;
    TArray<RoomPtr> roomListB;

    if (forceAccessbilityFromMainRoom)
    {
        for (const auto& room : allRooms)
        {
            if (room->isAccessibleFromMainRoom_)
            {
                roomListB.Add(room);
            }
            else
            {
                roomListA.Add(room);
            }
        }
    }
    else
    {
        roomListA = allRooms;
        roomListB = allRooms;
    }

    int32 bestDistance = -1;
    Coord bestTileA;
    Coord bestTileB;
    RoomPtr bestRoomA;
    RoomPtr bestRoomB;
    bool possibleConnectionFound = false;

    for (const auto& roomA : roomListA)
    {
        if (forceAccessbilityFromMainRoom == false)
        {
            possibleConnectionFound = false;
            if (roomA->connectedRooms_.Num() > 0)
            {
                continue;
            }
        }

        for (const auto& roomB : roomListB)
        {
            if (roomA == roomB || roomA->IsConnected(roomB))
            {
                continue;
            }

            for (int32 tileIndexA = 0; tileIndexA < roomA->edgeTiles_.Num(); tileIndexA++)
            {
                for (int32 tileIndexB = 0; tileIndexB < roomB->edgeTiles_.Num(); tileIndexB++)
                {
                    Coord tileA = roomA->edgeTiles_[tileIndexA];
                    Coord tileB = roomB->edgeTiles_[tileIndexB];
                    int32 distanceBetweenRooms = FVector2D::Distance(FVector2D(tileA.tileX_, tileA.tileY_), FVector2D(tileB.tileX_, tileB.tileY_));

                    if (distanceBetweenRooms < bestDistance || !possibleConnectionFound)
                    {
                        bestDistance = distanceBetweenRooms;
                        possibleConnectionFound = true;
                        bestTileA = tileA;
                        bestTileB = tileB;
                        bestRoomA = roomA;
                        bestRoomB = roomB;
                    }
                }
            }
        }

        if (possibleConnectionFound && forceAccessbilityFromMainRoom == false)
        {
            CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
        }
    }
    
    if (possibleConnectionFound && forceAccessbilityFromMainRoom)
    {
        CreatePassage(bestRoomA, bestRoomB, bestTileA, bestTileB);
        ConnectedClosestRooms(allRooms, true);
    }

    if (forceAccessbilityFromMainRoom == false)
    {
        ConnectedClosestRooms(allRooms, true);
    }
}

void ARandomMapGenActor::CreatePassage(RoomPtr roomA, RoomPtr roomB, const Coord& tileA, const Coord& tileB)
{
    Room::ConnectRooms(roomA, roomB);

    TArray<Coord> line = GetLine(tileA, tileB);
    /*
    for (int32 i = 0; i < line.Num() - 1; i++)
    {
        FVector offset(0.f, 0.f, 2.f);
        FVector startPos = CoordToWorldPosition(line[i]) + offset;
        FVector endPos = CoordToWorldPosition(line[i+1]) + offset;
        DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, true);
    }*/

    for (const auto& tile : line)
    {
        DrawCircle(tile, PassageSize);
    }
}

void ARandomMapGenActor::DrawCircle(const Coord& tile, int32 r)
{
    for (int32 x = -r ; x < r; x++)
    {
        for (int32 y = -r ; y < r; y++)
        {
            if (FMath::Pow(x, 2) + FMath::Pow(y, 2) <= FMath::Pow(r, 2))
            {
                int32 drawX = tile.tileX_ + x;
                int32 drawY = tile.tileY_ + y;
                if (IsInMap(drawX, drawY))
                {
                    maps_[drawY][drawX] = LIVE_CELL;
                }
            }
        }
    }
}

TArray<Coord> ARandomMapGenActor::GetLine(Coord from, Coord to) const
{
    TArray<Coord> line;

    int32 x = from.tileX_;
    int32 y = from.tileY_;

    int32 dx = to.tileX_ - x;
    int32 dy = to.tileY_ - y;

    bool inverted = false;
    int32 step = FMath::Sign<int32>(dx);
    int32 gradientStep = FMath::Sign<int32>(dy);

    int32 longest = FMath::Abs(dx);
    int32 shortest = FMath::Abs(dy);

    if (longest < shortest)
    {
        inverted = true;
        Swap(shortest, longest);
        Swap(step, gradientStep);
    }

    int32 gradientAccumlation = longest / 2;
    for (int32 i = 0; i < longest; i++)
    {
        line.AddUnique(Coord(x, y));

        if (inverted)
        {
            y += step;
        }
        else
        {
            x += step;
        }

        gradientAccumlation += shortest;
        if (gradientAccumlation >= longest)
        {
            if (inverted)
            {
                x += gradientStep;
            }
            else
            {
                y += gradientStep;
            }
            gradientAccumlation -= longest;
        }
    }

    line.AddUnique(Coord(to.tileX_, to.tileY_));

    return line;
}

void ARandomMapGenActor::TriangulateSquare(SquarePtr square)
{
    switch (square->configuration_)
    {
    case 0:
        break;

        // 1point
    case 1:
        MeshFromPoints({ square->centerLeft_, square->centerBottom_, square->bottomLeft_});
        break;
    case 2:
        MeshFromPoints({ square->bottomRight_, square->centerBottom_, square->centerRight_});
        break;
    case 4:
        MeshFromPoints({ square->topRight_, square->centerRight_, square->centerTop_});
        break;
    case 8:
        MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerLeft_ });
        break;

        // 2 points
    case 3:
        MeshFromPoints({ square->centerRight_, square->bottomRight_, square->bottomLeft_, square->centerLeft_ });
        break;
    case 6:
        MeshFromPoints({ square->centerTop_, square->topRight_, square->bottomRight_, square->centerBottom_});
        break;
    case 9:
        MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerBottom_, square->bottomLeft_});
        break;
    case 12:
        MeshFromPoints({ square->topLeft_, square->topRight_, square->centerRight_, square->centerLeft_ });
        break;
    case 5:
        MeshFromPoints({ square->centerTop_, square->topRight_, square->centerRight_, square->centerBottom_, square->bottomLeft_, square->centerLeft_});
        break;
    case 10:
        MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerRight_, square->bottomRight_, square->centerBottom_, square->centerLeft_});
        break;

        // 3 points
    case 7:
        MeshFromPoints({ square->centerTop_, square->topRight_, square->bottomRight_, square->bottomLeft_, square->centerLeft_});
        break;
    case 11:
        MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerRight_, square->bottomRight_, square->bottomLeft_});
        break;
    case 13:
        MeshFromPoints({ square->topLeft_, square->topRight_, square->centerRight_, square->centerBottom_, square->bottomLeft_});
        break;
    case 14:
        MeshFromPoints({ square->topLeft_, square->topRight_, square->bottomRight_, square->centerBottom_, square->centerLeft_});
        break;

        // 4 points
    case 15:
        MeshFromPoints({ square->topLeft_, square->topRight_, square->bottomRight_, square->bottomLeft_ });

        checkedVertices_.Add(square->topLeft_->vertexIndex_);
        checkedVertices_.Add(square->topRight_->vertexIndex_);
        checkedVertices_.Add(square->bottomRight_->vertexIndex_);
        checkedVertices_.Add(square->bottomLeft_->vertexIndex_);

        break;
    }

}

void ARandomMapGenActor::MeshFromPoints(const TArray<NodePtr>& points)
{
    AssignVertices(points);

    int32 count = points.Num();
    if (count >= 3)
    {
        CreateTriangle(points[0], points[1], points[2]);
    }
    if (count >= 4)
    {
        CreateTriangle(points[0], points[2], points[3]);
    }
    if (count >= 5)
    {
        CreateTriangle(points[0], points[3], points[4]);
    }
    if (count >= 6)
    {
        CreateTriangle(points[0], points[4], points[5]);
    }
}

void ARandomMapGenActor::AssignVertices(const TArray<NodePtr>& points)
{
    for (int32 i = 0; i < points.Num(); i++)
    {
        if (points[i]->vertexIndex_ == INDEX_NONE)
        {
            points[i]->vertexIndex_ = vertices_.Num();
            vertices_.Add(points[i]->position_);
        }
    }
}

void ARandomMapGenActor::CreateTriangle(NodePtr a, NodePtr b, NodePtr c)
{
    triangles_.Add(a->vertexIndex_);
    triangles_.Add(b->vertexIndex_);
    triangles_.Add(c->vertexIndex_);

    TrianglePtr triangle = MakeShared<Triangle>(a->vertexIndex_, b->vertexIndex_, c->vertexIndex_);
    AddTriangleToMap(triangle->vertexIndexA_, triangle);
    AddTriangleToMap(triangle->vertexIndexB_, triangle);
    AddTriangleToMap(triangle->vertexIndexC_, triangle);
}

void ARandomMapGenActor::AddTriangleToMap(int32 vertexKey, TrianglePtr triangle)
{
    auto& list = triangleMaps_.FindOrAdd(vertexKey);
    list.Add(triangle);
}

bool ARandomMapGenActor::IsOutlineEdge(int32 vertexA, int32 vertexB)
{
    int32 sharedTriangleCount = 0;

    auto it = triangleMaps_.Find(vertexA);
    if (it)
    {
        for (int32 i = 0; i < it->Num(); i++)
        {
            if ((*it)[i]->Contains(vertexB))
            {
                sharedTriangleCount++;
                if (sharedTriangleCount > 1)
                {
                    break;
                }
            }
        }
    }

    return sharedTriangleCount == 1;
}

int32 ARandomMapGenActor::GetConnectedOutlineVertex(int32 vertexIndex)
{
    auto it = triangleMaps_.Find(vertexIndex);
    if (it)
    {
         for (int32 i = 0; i < it->Num(); i++)
         {
             TrianglePtr triangle = (*it)[i];

             for (int32 j = 0; j< triangle->vertices_.Num(); j++)
             {
                 int32 vertexB = triangle->vertices_[j];
                 if (vertexB == vertexIndex || checkedVertices_.Contains(vertexB))
                 {
                     continue;
                 }

                 if (IsOutlineEdge(vertexIndex, vertexB))
                 {
                     return vertexB;
                 }
             }
         }       
    }

    return INDEX_NONE;
}

void ARandomMapGenActor::CalculateMeshOutlines()
{
    for (int32 vertexIndex = 0; vertexIndex < vertices_.Num(); vertexIndex++)
    {
        if (!checkedVertices_.Contains(vertexIndex))
        {
            int32 newOutlineVertex = GetConnectedOutlineVertex(vertexIndex);
            if (newOutlineVertex != INDEX_NONE)
            {
                checkedVertices_.Add(vertexIndex);

                TArray<int32> newOutline;
                newOutline.Add(vertexIndex);
                outlines_.Add(newOutline);

                FollowOutline(newOutlineVertex, outlines_.Num() - 1);
                outlines_[outlines_.Num() - 1].Add(vertexIndex);
            }
        }
    }
}

void ARandomMapGenActor::FollowOutline(int32 vertexIndex, int32 outlineIndex)
{
    outlines_[outlineIndex].Add(vertexIndex);
    checkedVertices_.Add(vertexIndex);
    int32 nextVertexIndex = GetConnectedOutlineVertex(vertexIndex);
    if (nextVertexIndex != INDEX_NONE)
    {
        FollowOutline(nextVertexIndex, outlineIndex);
    }
}

TArray<TArray<Coord>> ARandomMapGenActor::GetRegions(int32 tileType) const
{
    TArray<TArray<Coord>> regions;
    TArray<TArray<int32>> mapFlags;
    InitArray2D(mapFlags, Height, Width);

    for (int32 x = 0; x < Width; x++)
    {
        for (int32 y = 0; y < Height; y++)
        {
            if (mapFlags[y][x] == 0 && maps_[y][x] == tileType)
            {
                TArray<Coord> newRegion = GetRegionTiles(x, y);
                regions.Add(newRegion);

                for (const auto& coord : newRegion)
                {
                    mapFlags[coord.tileY_][coord.tileX_] = 1;
                }
            }
        }
    }

    return regions;
}

TArray<Coord> ARandomMapGenActor::GetRegionTiles(int32 startX, int32 startY) const
{
    TArray<Coord> tiles;
    TArray<TArray<int32>> mapFlags;
    InitArray2D(mapFlags, Height, Width);

    TQueue<Coord> queue;
    queue.Enqueue(Coord(startX, startY));
    mapFlags[startY][startX] = 1;

    int32 tileType = maps_[startY][startX];

    while (queue.IsEmpty() == false)
    {
        Coord tile;
        queue.Dequeue(tile);
        tiles.Add(tile);

        for (int32 x = tile.tileX_ - 1 ; x <= tile.tileX_ + 1; x++)
        {
            for (int32 y = tile.tileY_ - 1; y <= tile.tileY_ + 1; y++)
            {
                if (IsInMap(x, y) && (x == tile.tileX_ || y == tile.tileY_))
                {
                    if (mapFlags[y][x] == 0 && maps_[y][x] == tileType)
                    {
                        mapFlags[y][x] = 1;
                        queue.Enqueue(Coord(x, y));
                    }
                }
            }
        }
    }
    return tiles;
}

bool ARandomMapGenActor::IsInMap(int32 x, int32 y) const
{
    return x >= 0 && x < Width && y >= 0 && y < Height;
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
        , 0.f);
}

void ARandomMapGenActor::SimulateMap()
{
    TArray<TArray<int32> > newMap;
    InitArray2D(newMap, Height, Width);

    newMap = maps_;

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++)
        {
            int newVal = CountAliveNeighbours(x, y);
            if (maps_[y][x] == DEAD_CELL)
            {
                if (newVal < BirthCount)
                {
                    newMap[y][x] = LIVE_CELL;
                }
            }
            else
            {
                if (newVal > DeathCount)
                {
                    newMap[y][x] = DEAD_CELL;
                }
            }
        }
    }

    maps_ = newMap;
}

void ARandomMapGenActor::InitializeMap()
{
    const auto& originLoc = GetActorLocation();

    for (int32 y = 0 ; y < Height; y++)
    {
        for (int32 x = 0; x < Width; x++)
        {
            FString name = FString::Format(TEXT("cell_{0}"), { y * Width + x });
            auto meshComp = NewObject<UStaticMeshComponent>(this, *name);
            if (meshComp)
            {
                meshComp->RegisterComponentWithWorld(GetWorld());
                
                FVector loc(originLoc.X + (x * 100), originLoc.Y + (y * 100), originLoc.Z);
                meshComp->SetWorldLocation(loc);

                meshs_.Add(meshComp);
            }
        }
    }

    //proceduralMeshComp_ = NewObject<UProceduralMeshComponent>(this, "proceduralMeshComp");
    //proceduralMeshComp_->RegisterComponentWithWorld(GetWorld());

    TArray<TArray<int32> > map; //create map to return
    InitArray2D(map, Height, Width);

    for (int y = 0; y < Height; y++)
    {
        for (int x = 0; x < Width; x++) //initialising values here
        {
            if (y == 0 || x == 0 || y == Height - 1 || x == Width - 1)
            {
                map[y][x] = DEAD_CELL;
            }
            else if (FMath::RandRange(0.f, 100.f) < SpawnProbability) //it has a chance of being a wall or not
            {
                map[y][x] = DEAD_CELL;
            }
            else
            {
                map[y][x] = LIVE_CELL;
            }
        }
    }

    maps_ = map;
}

void ARandomMapGenActor::DeInitializeMap()
{
    for (auto& mesh : meshs_)
    {
        if (mesh)
        {
            mesh->DestroyComponent();
        }
    }

    if (proceduralMeshComp_)
    {
        //proceduralMeshComp_->DestroyComponent();
        proceduralMeshComp_->ClearAllMeshSections();
    }

    meshs_.Empty();
    maps_.Empty();
    vertices_.Empty();
    triangles_.Empty();
    outlines_.Empty();
    checkedVertices_.Empty();
    triangleMaps_.Empty();

    FlushPersistentDebugLines(GetWorld());
}

void ARandomMapGenActor::DrawCells()
{
    for (int h = 0; h < Height; h++)
    {
        for (int w = 0; w < Width; w++) //initialising values here
        {
            if (maps_[h][w])
            {
                meshs_[h * Width + w]->SetStaticMesh(FloorMesh);
            }
            else
            {
                meshs_[h * Width + w]->SetStaticMesh(WallMesh);
            }
        }
    }
}
