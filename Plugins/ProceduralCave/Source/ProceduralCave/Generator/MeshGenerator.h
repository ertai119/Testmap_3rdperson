// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"

class UNodeBase
{
public:
    FVector position_;
    int32 vertexIndex_ = INDEX_NONE;

    virtual ~UNodeBase() {}

    explicit UNodeBase(FVector pos);
};

using NodePtr = TSharedPtr<UNodeBase>;

class UControlNode : public UNodeBase
{

public:
    bool active_ = false;
    NodePtr above_, right_;

    virtual ~UControlNode() {}

    UControlNode(FVector pos, bool active, float squareSize);
};
using ControlNodePtr = TSharedPtr<UControlNode>;

class Triangle 
{
public:
    int32 vertexIndexA_;
    int32 vertexIndexB_;
    int32 vertexIndexC_;
    TArray<int32> vertices_;

    Triangle(int32 a, int32 b, int32 c);

    bool Contains(int32 vertexIndex) const;
};
using TrianglePtr = TSharedPtr<Triangle>;

class USquare
{
public:
    ControlNodePtr topLeft_, topRight_, bottomRight_, bottomLeft_;
    NodePtr centerTop_, centerRight_, centerBottom_, centerLeft_;
    int32 configuration_ = 0;

    USquare(ControlNodePtr topLeft, ControlNodePtr topRight, ControlNodePtr bottomRight, ControlNodePtr bottomLeft);
};
using SquarePtr = TSharedPtr<USquare>;

class MeshGenerator
{
public:
    TArray<TArray<SquarePtr>> squares_;
	TArray<FVector> vertices_;
	TArray<int32> triangles_;
	TArray<TArray<int32>> outlines_;
	TSet<int32> checkedVertices_;
    TMap<int32, TArray<TrianglePtr>> triangleMaps_;

    MeshGenerator(TArray<TArray<int32>> map, float offsetZ, float squareSize);

    void CalculateTriangle(float wallHeight);
    
private:
	void AssignVertices(const TArray<NodePtr>& points);
	void CreateTriangle(NodePtr a, NodePtr b, NodePtr c);
	void AddTriangleToMap(int32 vertexKey, TrianglePtr triangle);
	void CalculateMeshOutlines();
	int32 GetConnectedOutlineVertex(int32 vertexIndex);
	void FollowOutline(int32 vertexIndex, int32 outlineIndex);
	bool IsOutlineEdge(int32 vertexA, int32 vertexB);
	void TriangulateSquare(SquarePtr square, float offsetHeight);
	void MeshFromPoints(const TArray<NodePtr>& points);
};
