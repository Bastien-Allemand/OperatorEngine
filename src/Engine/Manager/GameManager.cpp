#include "pch.h"
#include "GameManager.h"
#include "SystemManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
	return (m_instance == nullptr) ? m_instance = new GameManager() : m_instance;
}

GameManager::~GameManager()
{

}

Entity* GameManager::GetEntity(Id _entity)
{
	for (auto entite : m_entities)
		if (entite->id == _entity)
			return entite;
}

Id* GameManager::AddEntity()
{
	Entity* newEntity = new Entity();
	newEntity->id.index = m_entities.size();
	m_entities.push_back(newEntity);
	return &newEntity->id;
}

void GameManager::Update()
{
	m_systemManager->Update();
}