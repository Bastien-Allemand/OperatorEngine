#pragma once

struct vertex
{
	Vector3f pos;
	Vector3f normal;
	Vector2f uv;

};

class Geometry
{
	public:
	Geometry() = default;
	~Geometry() = default;
	Vector<vertex> vertices;
	Vector<unsigned int> indices;

	void BuildQuad();
	void BuildVertexBuffer();
};

