#include "RoomGenerator.h"

#define LIVE_CELL (0)
#define DEAD_CELL (1)

Room::Room(const TArray<Coord>& tiles, const TArray<TArray<int32>>& map)
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

void Room::ConnectRooms(RoomPtr roomA, RoomPtr roomB)
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

bool Room::IsConnected(RoomPtr room)
{
	return connectedRooms_.Contains(room);
}

void Room::SetAccessibleFromMainRoom()
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

URoomGenerator::URoomGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void URoomGenerator::InitializeMap(int32 height, int32 width, float spawnProbability)
{
	height_ = height;
	width_ = width;

	maps_.Empty();
	InitArray2D(maps_, height, width);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++) //initialising values here
		{
			if (y == 0 || x == 0 || y == height - 1 || x == width - 1)
			{
				maps_[y][x] = DEAD_CELL;
			}
			else if (FMath::RandRange(0.f, 100.f) < spawnProbability) //it has a chance of being a wall or not
			{
				maps_[y][x] = DEAD_CELL;
			}
			else
			{
				maps_[y][x] = LIVE_CELL;
			}
		}
	}
}

void URoomGenerator::SimulateMap(int32 birthCount, int32 deathCount)
{
	TArray<TArray<int32>> newMap;
	InitArray2D(newMap, height_, width_);

	newMap = maps_;

	for (int y = 0; y < height_; y++)
	{
		for (int x = 0; x < width_; x++)
		{
			int newVal = CountAliveNeighbours(x, y);
			if (maps_[y][x] == DEAD_CELL)
			{
				if (newVal < birthCount)
				{
					newMap[y][x] = LIVE_CELL;
				}
			}
			else
			{
				if (newVal > deathCount)
				{
					newMap[y][x] = DEAD_CELL;
				}
			}
		}
	}

	maps_ = newMap;
}

int32 URoomGenerator::CountAliveNeighbours(int32 x, int32 y)
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
			else if (neighbour_x < 0 || neighbour_y < 0 || neighbour_x >= width_ || neighbour_y >= height_)
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

void URoomGenerator::ProcessMap(int32 wallThreshold, int32 roomThreshold, int32 wallThresholdAfterGen, int32 roomThresholdAfterGen, int32 passgeSize)
{
	passageSize_ = passgeSize;

	TArray<TArray<Coord>> wallRegions = GetRegions(DEAD_CELL);
	for (const auto& coordList : wallRegions)
	{
		if (coordList.Num() < wallThreshold)
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
		if (coordList.Num() < roomThreshold)
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
		if (coordList.Num() < wallThresholdAfterGen)
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
		if (coordList.Num() < roomThresholdAfterGen)
		{
			for (const auto& coord : coordList)
			{
				maps_[coord.tileY_][coord.tileX_] = DEAD_CELL;
			}
		}
	}
}

TArray<TArray<Coord>> URoomGenerator::GetRegions(int32 tileType) const
{
	TArray<TArray<Coord>> regions;
	TArray<TArray<int32>> mapFlags;
	InitArray2D(mapFlags, height_, width_);

	for (int32 x = 0; x < width_; x++)
	{
		for (int32 y = 0; y < height_; y++)
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

TArray<Coord> URoomGenerator::GetRegionTiles(int32 startX, int32 startY) const
{
	TArray<Coord> tiles;
	TArray<TArray<int32>> mapFlags;
	InitArray2D(mapFlags, height_, width_);

	TQueue<Coord> queue;
	queue.Enqueue(Coord(startX, startY));
	mapFlags[startY][startX] = 1;

	int32 tileType = maps_[startY][startX];

	while (queue.IsEmpty() == false)
	{
		Coord tile;
		queue.Dequeue(tile);
		tiles.Add(tile);

		for (int32 x = tile.tileX_ - 1; x <= tile.tileX_ + 1; x++)
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

bool URoomGenerator::IsInMap(int32 x, int32 y) const
{
	return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void URoomGenerator::ConnectedClosestRooms(const TArray<RoomPtr>& allRooms, bool forceAccessbilityFromMainRoom /*= false*/)
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

void URoomGenerator::CreatePassage(RoomPtr roomA, RoomPtr roomB, const Coord& tileA, const Coord& tileB)
{
	Room::ConnectRooms(roomA, roomB);

	TArray<Coord> line = GetLine(tileA, tileB);

	/*for (int32 i = 0; i < line.Num() - 1; i++)
	{
		FVector offset(0.f, 0.f, 2.f);
		FVector startPos = CoordToWorldPosition(line[i]) + offset;
		FVector endPos = CoordToWorldPosition(line[i + 1]) + offset;
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Red, true);
	}*/

	bool found = false;
	for (const auto& info : passageInfo_)
	{
		if ((info.start_ == tileA && info.end_ == tileB) || (info.start_ == tileB && info.end_ == tileA))
		{
			found = true;
			break;
		}
	}

	TArray<Coord> drawtiles;
	for (const auto& tile : line)
	{
		auto temptiles = DrawCircle(tile, passageSize_);
		for (const auto& temp : temptiles)
		{
			drawtiles.AddUnique(temp);
		}
	}

	if (found == false && drawtiles.Num() > 0)
	{
		PassageInfo info;
		info.start_ = tileA;
		info.end_ = tileB;
		info.tiles_ = drawtiles;
		passageInfo_.Add(info);
	}
}

TArray<Coord> URoomGenerator::GetLine(Coord from, Coord to) const
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

TArray<Coord> URoomGenerator::DrawCircle(const Coord& tile, int32 r)
{
	TArray<Coord> retTiles;
	for (int32 x = -r; x <= r; x++)
	{
		for (int32 y = -r; y <= r; y++)
		{
			if (FMath::Pow(x, 2) + FMath::Pow(y, 2) <= FMath::Pow(r, 2))
			{
				int32 drawX = tile.tileX_ + x;
				int32 drawY = tile.tileY_ + y;
				if (IsInMap(drawX, drawY))
				{
					retTiles.Add(Coord(drawX, drawY));
					maps_[drawY][drawX] = LIVE_CELL;
				}
			}
		}
	}
	return retTiles;
}