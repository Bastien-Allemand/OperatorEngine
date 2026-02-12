#pragma once

struct vertex
{
	Vector3f pos;
	Vector4f color;
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

