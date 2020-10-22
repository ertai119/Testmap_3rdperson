#include "MeshGenerator.h"
#include <DrawDebugHelpers.h>

UNodeBase::UNodeBase(FVector pos)
{
	position_ = pos;
}

UControlNode::UControlNode(FVector pos, bool active, float squareSize) : UNodeBase(pos)
{
	active_ = active;
	above_ = MakeShared<UNodeBase>(pos + FVector(0.f, 1.f, 0.f) * squareSize * 0.5f);
	right_ = MakeShared<UNodeBase>(pos + FVector(1.f, 0.f, 0.f) * squareSize * 0.5f);
}

Triangle::Triangle(int32 a, int32 b, int32 c)
{
	vertexIndexA_ = a;
	vertexIndexB_ = b;
	vertexIndexC_ = c;

	vertices_.SetNum(3);
	vertices_[0] = a;
	vertices_[1] = b;
	vertices_[2] = c;
}

bool Triangle::Contains(int32 vertexIndex) const
{
	return vertexIndexA_ == vertexIndex || vertexIndexB_ == vertexIndex || vertexIndexC_ == vertexIndex;
}


USquare::USquare(ControlNodePtr topLeft, ControlNodePtr topRight, ControlNodePtr bottomRight, ControlNodePtr bottomLeft)
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

UMeshGenerator::UMeshGenerator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UMeshGenerator::InitData(TArray<TArray<int32>> map, float offsetZ, float squareSize)
{
	int32 nodeCountY = map.Num();
	if (nodeCountY == 0)
	{
		return;
	}
	int32 nodeCountX = map[0].Num();
	if (nodeCountX == 0)
	{
		return;
	}

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
				, offsetZ);

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

void UMeshGenerator::CalculateTriangle(float wallHeight, bool includeFloor)
{
	for (int32 y = 0; y < squares_.Num(); y++)
	{
		for (int32 x = 0; x < squares_[0].Num(); x++)
		{
			TriangulateSquare(squares_[y][x], wallHeight, true);
		}
	}

	TArray<FVector> dumpVertices;
	TArray<int32> dumpTriangles;
	if (includeFloor)
	{
		dumpVertices = vertices_;
		dumpTriangles = triangles_;
	}
	
	CalculateMeshOutlines();

	TArray<FVector> wallVertices;
	TArray<int32> wallTriangles;
	int32 startOffset = vertices_.Num();

	for (const auto& outline : outlines_)
	{
		for (int32 i = 0; i < outline.Num() - 1; i++)
		{
			int32 startIndex = wallVertices.Num();
			wallVertices.Add(vertices_[outline[i]]); // left
			wallVertices.Add(vertices_[outline[i + 1]]); // right
			wallVertices.Add(vertices_[outline[i]] - FVector::UpVector * wallHeight); // bottom left
			wallVertices.Add(vertices_[outline[i + 1]] - FVector::UpVector * wallHeight); // bottom right

			wallTriangles.Add(startOffset + startIndex + 0);
			wallTriangles.Add(startOffset + startIndex + 2);
			wallTriangles.Add(startOffset + startIndex + 3);

			wallTriangles.Add(startOffset + startIndex + 3);
			wallTriangles.Add(startOffset + startIndex + 1);
			wallTriangles.Add(startOffset + startIndex + 0);
		}
	}

	vertices_.Append(wallVertices);
	triangles_.Append(wallTriangles);

	if (includeFloor)
	{
		TArray<int32> floorTriangles;
		FVector offset = FVector::ZeroVector + FVector(0.f, 0.f, -wallHeight);
		for (auto& vertex : dumpVertices)
		{
			vertex += offset;
		}

		int32 floorIndexStartOffset = vertices_.Num();
		for (int32 i = 0; i < dumpTriangles.Num(); i += 3)
		{
			floorTriangles.Add(floorIndexStartOffset + dumpTriangles[i]);
			floorTriangles.Add(floorIndexStartOffset + dumpTriangles[i + 2]);
			floorTriangles.Add(floorIndexStartOffset + dumpTriangles[i + 1]);
		}

		vertices_.Append(dumpVertices);
		triangles_.Append(floorTriangles);
	}
}

void UMeshGenerator::TriangulateSquare(SquarePtr square, float offsetHeight, bool ccw /*= true*/)
{
	switch (square->configuration_)
	{
	case 0:
		break;

		// 1point
	case 1:
		MeshFromPoints({ square->centerLeft_, square->centerBottom_, square->bottomLeft_ }, ccw);
		break;
	case 2:
		MeshFromPoints({ square->bottomRight_, square->centerBottom_, square->centerRight_ }, ccw);
		break;
	case 4:
		MeshFromPoints({ square->topRight_, square->centerRight_, square->centerTop_ }, ccw);
		break;
	case 8:
		MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerLeft_ }, ccw);
		break;

		// 2 points
	case 3:
		MeshFromPoints({ square->centerRight_, square->bottomRight_, square->bottomLeft_, square->centerLeft_ }, ccw);
		break;
	case 6:
		MeshFromPoints({ square->centerTop_, square->topRight_, square->bottomRight_, square->centerBottom_ }, ccw);
		break;
	case 9:
		MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerBottom_, square->bottomLeft_ }, ccw);
		break;
	case 12:
		MeshFromPoints({ square->topLeft_, square->topRight_, square->centerRight_, square->centerLeft_ }, ccw);
		break;
	case 5:
		MeshFromPoints({ square->centerTop_, square->topRight_, square->centerRight_, square->centerBottom_, square->bottomLeft_, square->centerLeft_ }, ccw);
		break;
	case 10:
		MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerRight_, square->bottomRight_, square->centerBottom_, square->centerLeft_ }, ccw);
		break;

		// 3 points
	case 7:
		MeshFromPoints({ square->centerTop_, square->topRight_, square->bottomRight_, square->bottomLeft_, square->centerLeft_ }, ccw);
		break;
	case 11:
		MeshFromPoints({ square->topLeft_, square->centerTop_, square->centerRight_, square->bottomRight_, square->bottomLeft_ }, ccw);
		break;
	case 13:
		MeshFromPoints({ square->topLeft_, square->topRight_, square->centerRight_, square->centerBottom_, square->bottomLeft_ }, ccw);
		break;
	case 14:
		MeshFromPoints({ square->topLeft_, square->topRight_, square->bottomRight_, square->centerBottom_, square->centerLeft_ }, ccw);
		break;

		// 4 points
	case 15:
		MeshFromPoints({ square->topLeft_, square->topRight_, square->bottomRight_, square->bottomLeft_ }, ccw);

		checkedVertices_.Add(square->topLeft_->vertexIndex_);
		checkedVertices_.Add(square->topRight_->vertexIndex_);
		checkedVertices_.Add(square->bottomRight_->vertexIndex_);
		checkedVertices_.Add(square->bottomLeft_->vertexIndex_);

		break;
	}
}

void UMeshGenerator::MeshFromPoints(const TArray<NodePtr>& points, bool ccw)
{
	AssignVertices(points);

	int32 count = points.Num();
	if (count >= 3)
	{
		if (ccw)
		{
			CreateTriangle(points[0], points[1], points[2]);
		}
		else
		{
			CreateTriangle(points[0], points[2], points[1]);
		}		
	}
	if (count >= 4)
	{
		if (ccw)
		{
			CreateTriangle(points[0], points[2], points[3]);
		}
		else
		{
			CreateTriangle(points[0], points[3], points[2]);
		}
		
	}
	if (count >= 5)
	{
		if (ccw)
		{
			CreateTriangle(points[0], points[3], points[4]);
		}
		else
		{
			CreateTriangle(points[0], points[4], points[3]);
		}
		
	}
	if (count >= 6)
	{
		if (ccw)
		{
			CreateTriangle(points[0], points[4], points[5]);
		}
		else
		{
			CreateTriangle(points[0], points[5], points[4]);
		}		
	}
}

void UMeshGenerator::AssignVertices(const TArray<NodePtr>& points)
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

void UMeshGenerator::CreateTriangle(NodePtr a, NodePtr b, NodePtr c)
{
	triangles_.Add(a->vertexIndex_);
	triangles_.Add(b->vertexIndex_);
	triangles_.Add(c->vertexIndex_);

	TrianglePtr triangle = MakeShared<Triangle>(a->vertexIndex_, b->vertexIndex_, c->vertexIndex_);
	AddTriangleToMap(triangle->vertexIndexA_, triangle);
	AddTriangleToMap(triangle->vertexIndexB_, triangle);
	AddTriangleToMap(triangle->vertexIndexC_, triangle);
}

void UMeshGenerator::AddTriangleToMap(int32 vertexKey, TrianglePtr triangle)
{
	auto& list = triangleMaps_.FindOrAdd(vertexKey);
	list.Add(triangle);
}

void UMeshGenerator::CalculateMeshOutlines()
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

int32 UMeshGenerator::GetConnectedOutlineVertex(int32 vertexIndex)
{
	auto it = triangleMaps_.Find(vertexIndex);
	if (it)
	{
		for (int32 i = 0; i < it->Num(); i++)
		{
			TrianglePtr triangle = (*it)[i];

			for (int32 j = 0; j < triangle->vertices_.Num(); j++)
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

void UMeshGenerator::FollowOutline(int32 vertexIndex, int32 outlineIndex)
{
	outlines_[outlineIndex].Add(vertexIndex);
	checkedVertices_.Add(vertexIndex);
	int32 nextVertexIndex = GetConnectedOutlineVertex(vertexIndex);
	if (nextVertexIndex != INDEX_NONE)
	{
		FollowOutline(nextVertexIndex, outlineIndex);
	}
}

bool UMeshGenerator::IsOutlineEdge(int32 vertexA, int32 vertexB)
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

