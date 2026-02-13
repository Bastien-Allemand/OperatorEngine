#include "pch.h"
#include "SystemManager.h"
#include "System.h"

bool SystemManager::AddSystem(System* _system)
{
	m_systems.push_back(_system);
	return true;
}
void SystemManager::Update()
{
	for (auto& system : m_systems)
	{
		system->Update();
	}
}