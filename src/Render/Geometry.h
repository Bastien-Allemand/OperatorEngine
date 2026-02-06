#pragma once

struct vertex
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;

};

class Geometry
{
	public:
	Geometry() = default;
	~Geometry() = default;
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;

	void BuildQuad();
	void BuildVertexBuffer();
};

