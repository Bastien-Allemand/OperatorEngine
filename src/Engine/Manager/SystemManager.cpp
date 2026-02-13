#include "pch.h"
#include "SystemManager.h"
#include "System.h"
#include "Entity.h"
#include "Component.h"
#include "System/MeshSystem.h"

void SystemManager::FilterEntity(Entity* _entity)
{
	for (Component* component : _entity->componants)
	{
		switch (component->type)
		{
			case Component::ComponentType::Mesh:
				m_meshSystem->AddEntity(_entity);
				break;
		default:
			break;
		}
	}
}
void SystemManager::Update()
{

}