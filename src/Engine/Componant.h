#pragma once
#include <string>
#include <unordered_map>
#include <iostream>

struct Componant
{
public:

	UINT id;
	virtual ~Componant() = default; // Indispensable !
};

struct Transform : public Componant
{
public:
	Vector3f position;

	Vector3f scale;

	Vector3f direction;
	Vector3f up;
	Vector3f right;
	Quaternion quaternion;
	Matrix4x4f rot;


	bool wolrdNeedsUpdate;
	bool invWolrdNeedsUpdate;
private:
	Matrix4x4f m_world;

	Matrix4x4f m_invworld;

public:
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

struct InputAction
{
    int keyCode;
    bool isPressed = false;    
    bool isJustPressed = false; 
    float value = 0.0f;
};

struct InputComponent : public Componant
{
    std::unordered_map<std::string, InputAction> actions;
	void BindAction(const std::string& _actionName, int key);
	bool IsActionPressed(const std::string& _actionName);
	bool IsActionJustPressed(const std::string& _actionName);
};
