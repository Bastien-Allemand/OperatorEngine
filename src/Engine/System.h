#pragma once
#include <vector>
#include <DirectXMath.h>
#include "GameManager.h"

class GameManager;

class System
{
public:
    System();
    virtual ~System() = default;
    virtual void Update(const std::vector<UINT>& entities, float deltaTime) = 0;
	virtual void AddEntity(UINT entityId);

    UINT id;
protected:
    GameManager* m_gameManager;
    std::vector<UINT> m_entities;
};

class TransformSystem : public System
{
public:
    TransformSystem() = default;
    void Update(const std::vector<UINT>& entities, float deltaTime) override;
};

class InputSystem : public System
{
public:
    InputSystem() = default;
	void Update(const std::vector<UINT>& entities, float deltaTime) override;
};