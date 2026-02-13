#pragma once
#include "Component.h"
#include "Geometry.h"
#include "Mesh.h"

struct MeshComponent : public Component
{
public:
	MeshComponent()
	{
		type = ComponentType::Mesh;
	}
	Geometry geometry;

	Mesh* mesh = nullptr;
	bool initialized = false;
	bool visible = true;
};

