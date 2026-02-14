#include "pch.h"
#include "SystemManager.h"
#include "System.h"
#include "System/MeshSystem.h"
#include "System/TransformSystem.h"

SystemManager::SystemManager()
{
	MeshSystem* meshSystem = new MeshSystem();
	TransformSystem* transformSystem = new TransformSystem();
	m_systems.push_back(meshSystem);
	m_systems.push_back(transformSystem);
}


void SystemManager::Update()
{
	for (ISystem* system : m_systems)
	{
		system->Update();
	}
}