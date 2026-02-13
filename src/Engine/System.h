#pragma once
#include "Manager/GameManager.h"

class GameManager;
class Entity;

class System
{
public:
    System();
    virtual ~System() = default;
    virtual void Update() = 0;
	virtual void AddEntity(UINT entityId);

    Entity* GetEntity(Id id);

    Id id;

	Vector<Entity*> m_entities; // Liste des entités que ce système gère
protected:
    //Vector<UINT> m_entities;
};

class TransformSystem : public System
{
public:
    TransformSystem() = default;
    void Update() override;
	void AddChildren(Transform& child);
	void AddParent(Transform& Parent);

    void Move();
};

class InputSystem : public System
{
public:
    InputSystem() = default;
	void Update() override;
};