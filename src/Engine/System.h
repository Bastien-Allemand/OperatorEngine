#pragma once
#include <vector>
#include <DirectXMath.h>
#include "GameManager.h"
class System
{
protected:
    GameManager* m_gameManager;
	std::vector<UINT> m_entities; // Liste des entités que ce système gère

public:
    System();
    virtual ~System() = default;
    virtual void Update(const std::vector<UINT>& entities, float deltaTime) = 0;
};

class TransformSystem : public System
{
public:
    TransformSystem() = default;
    void Update(const std::vector<UINT>& entities, float deltaTime) override;
};