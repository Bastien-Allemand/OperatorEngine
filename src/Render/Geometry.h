#pragma once

struct vertex
{
	Vector3f pos;
	Vector3f normal;
	Vector2f tex;
};

class Geometry
{
	public:
	Geometry() = default;
	~Geometry() = default;
	Vector<vertex> vertices;
	Vector<uint32> indices;

	void BuildBox();
	void BuildVertexBuffer();
};

