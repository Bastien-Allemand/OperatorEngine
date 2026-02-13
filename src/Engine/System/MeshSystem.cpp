#include "pch.h"
#include "MeshSystem.h"
#include "Entity.h"
#include "Component/MeshComponent.h"
#include "Manager/GameManager.h"
#include "Render/RenderEngine.h"
#include "Component/TransformComponent.h"

void MeshSystem::AddEntity(Entity* entity)
{
	m_entities.push_back(entity);
}

void MeshSystem::Update()
{
	MeshComponent* meshComponent = nullptr;
	TransformComponent* transformComponent = nullptr;
	for (auto entites : m_entities)
	{
		for (auto component : entites->componants )
		{
			meshComponent = static_cast<MeshComponent*>(component);
			if (meshComponent != nullptr)
			{
				if (!meshComponent->initialized)
				{
					GameManager::GetInstance()->GetRenderEngine()->InitMesh(meshComponent->mesh);
				}
				m_meshComponents[entites] = meshComponent;
				continue;
			}
			transformComponent = static_cast<TransformComponent*>(component);
			if (transformComponent != nullptr)
			{
				m_TransformComponents[entites] = transformComponent;
				continue;
			}
		}
	}
}
