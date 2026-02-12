#pragma once


struct Componant
{
public:

	UINT id;
	virtual ~Componant() = default; // Indispensable !
};

struct TransformComponant : public Componant
{
public:

	Vector3f position;

	Vector3f scale;

	Vector3f direction;
	Vector3f up;
	Vector3f right;
	Vector3f forward;

	Quaternion quaternion;
	Matrix4x4f rot;


	Transform worldtransform;
	Transform localTransform;

	bool wolrdNeedsUpdate;
	bool invWolrdNeedsUpdate;
private:
	Matrix4x4f m_world;

	Matrix4x4f m_invworld;

public:
	TransformComponant();
	Matrix4x4f& GetWorld();
	Matrix4x4f& GetInvWorld();
};

struct Velocity : public Componant
{
public:

	Vector3f position;

	Vector3f scale;

	Vector3f direction;
	Vector3f up;
	Vector3f right;
	Vector3f quaternion;
	Matrix4x4f rot;

	Matrix4x4f world;
};

