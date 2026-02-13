#pragma once
#include "Manager/GameManager.h"

class GameManager;
class Entity;

class System
{
public:
    System();
    virtual ~System() = default;
    virtual void Update(float deltaTime) = 0;
	virtual void AddEntity(UINT entityId);

    Entity* GetEntity(UINT id);

    UINT id;

	Vector<Entity*> m_entities; // Liste des entités que ce système gère
protected:
    Vector<UINT> m_entities;
};

class TransformSystem : public System
{
public:
    TransformSystem() = default;
    void Update(float deltaTime) override;
	void AddChildren(Transform& child);
	void AddParent(Transform& Parent);

    void Move();
};

class InputSystem : public System
{
public:
    InputSystem() = default;
	void Update(float deltaTime) override;
};