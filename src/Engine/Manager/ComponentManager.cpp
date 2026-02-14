#include "pch.h"
#include "ComponentManager.h"

void ComponentManager::RemoveEntity(uint32 _entityId)
{
	for (IComponentPool* pool : m_componentPools)
	{
		pool->Remove(_entityId);
	}
}
