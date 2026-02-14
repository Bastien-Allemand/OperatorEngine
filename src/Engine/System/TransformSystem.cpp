#include "pch.h"
#include "TransformSystem.h"
#include "Manager/GameManager.h"
#include "Component/TransformComponent.h"
#include "Manager/ComponentManager.h"
#include "ComponentPool.h"

void TransformSystem::Update()
{
	GameManager* gameManager = GameManager::GetInstance();
	ComponentPool<TransformComponent>* pool = gameManager->GComponentManager()->GPool<TransformComponent>();
	Vector<uint32> entities = pool->GEntityIds();
	for (uint32 entity : entities)
	{
	}
}
