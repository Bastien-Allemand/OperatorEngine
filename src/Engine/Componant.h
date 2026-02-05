#pragma once

using namespace DirectX;

struct Componant
{
public:

	UINT id;
};

struct Transform : public Componant
{
public:

	XMFLOAT3 position;

	XMFLOAT3 scale;

	XMFLOAT3 direction;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT4 quaternion;
	XMFLOAT4X4 rot;

	XMFLOAT4X4 world;
};