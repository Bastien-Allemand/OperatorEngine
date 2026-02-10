#include "pch.h"
#include "Geometry.h"

void Geometry::BuildQuad()
{
	vertices = {
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
		{{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
		{{-1.f, -1.f, 1.f}, {1.f, 1.f, 1.f}, {1.f, 1.f}}
	};
	indices = {
		0, 1, 2,
		2, 3, 0
	};
}

void Geometry::BuildVertexBuffer()
{

}