#include "pch.h"
#include "System.h"

System::System()
{
    m_gameManager = GameManager::GetInstance();
}

void System::AddEntity(UINT entity) {
    if (entity >= m_entities.size()) {
        m_entities.resize(entity + 1);
		m_entities[entity] = entity;
    }
}

Entity* System::GetEntity(UINT id)
{
    for (Entity* entity : m_entitiesss)
    {
        if (entity->id.first == id) return entity;
    }
    return nullptr;
}

