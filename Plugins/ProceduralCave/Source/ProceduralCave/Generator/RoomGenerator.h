// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

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

class PassageInfo
{
public:
	Coord start_;
	Coord end_;
	TArray<Coord> tiles_;
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
	Room(const TArray<Coord>& tiles, const TArray<TArray<int32>>& map);

	static void ConnectRooms(RoomPtr roomA, RoomPtr roomB);
	bool IsConnected(RoomPtr room);
	void SetAccessibleFromMainRoom();
};

class RoomGenerator
{
public:

	void InitializeMap(int32 height, int32 width, float spawnProbability);
	void SimulateMap(int32 birthCount, int32 deathCount);

	void ProcessMap(int32 wallThreshold, int32 roomThreshold, int32 wallThresholdAfterGen, int32 roomThresholdAfterGen, int32 passgeSize);
	TArray<TArray<Coord>> GetRegions(int32 tileType) const;
	const TArray<TArray<int32>>& GetMaps() const { return maps_; }

private:
	int32 CountAliveNeighbours(int32 x, int32 y);	
	TArray<Coord> GetRegionTiles(int32 startX, int32 startY) const;
	bool IsInMap(int32 x, int32 y) const;
	void ConnectedClosestRooms(const TArray<RoomPtr>& allRooms, bool forceAccessbilityFromMainRoom = false);
	void CreatePassage(RoomPtr roomA, RoomPtr roomB, const Coord& tileA, const Coord& tileB);
	TArray<Coord> GetLine(Coord from, Coord to) const;
	TArray<Coord> DrawCircle(const Coord& tile, int32 r);

	TArray<TArray<int32>> maps_;
	int32 height_;
	int32 width_;
	int32 passageSize_ = 3;
	TArray<PassageInfo> passageInfo_;
};
