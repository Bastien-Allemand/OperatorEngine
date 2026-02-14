#pragma once

class GameManager;
class Transform;

class ISystem
{
public:
    virtual ~ISystem() = default;
    virtual void Update() = 0;
};

class aTransformSystem
{
public:
    aTransformSystem() = default;
    void Update();
	void AddChildren(Transform& child);
	void AddParent(Transform& Parent);

    void Move();
};

class InputSystem
{
public:
    InputSystem() = default;
	void Update();
};