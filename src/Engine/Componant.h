#pragma once
#include "Core/Transform.h"

struct Componant
{
public:
	uint64 id;
	virtual ~Componant() = default;
};

struct TransformComponant : public Componant
{
public:
	TransformComponant() = default;

	Transform worldTransform;

	Transform localTransform;

	bool wolrdNeedsUpdate;
	bool invWolrdNeedsUpdate;

	Matrix4x4f m_world;
	Matrix4x4f m_invworld;
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


struct BoxCollider : public Componant
{
public:
	bool IsColliding = false;

	Vector3f center = { 0.0f, 0.0f, 0.0f };
	Vector3f size = { 1.0f, 1.0f, 1.0f };

	Vector3f min = { 0.0f, 0.0f, 0.0f };
	Vector3f max = { 0.0f, 0.0f, 0.0f };

};

struct InputAction
{
    int keyCode;
    bool isPressed = false;    
    bool isJustPressed = false; 
    float value = 0.0f;
};

struct InputComponent : public Componant
{
    std::unordered_map<String, InputAction> actions;
	void BindAction(const std::string& _actionName, int key);
	bool IsActionPressed(const std::string& _actionName);
	bool IsActionJustPressed(const std::string& _actionName);
};
