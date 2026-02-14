#pragma once
#include "Geometry.h"
#include "Mesh.h"

struct MeshComponent
{
public:
	Geometry geometry;

	Mesh* mesh = nullptr;
	bool initialized = false;
	bool visible = true;

	void InitMesh();

};

