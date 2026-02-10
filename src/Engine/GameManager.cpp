#include "pch.h"
#include "GameManager.h"

GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
	//return (m_instance == nullptr) ? new GameManager() : m_instance;
    if (m_instance == nullptr) {
        m_instance = new GameManager();
    }
    return m_instance;
}

GameManager::~GameManager()
{
	for (auto& entityComponents : m_componants) {
		for (auto& comp : entityComponents) {
			delete comp; // Supprime réellement l'objet en mémoire
		}
		entityComponents.clear();
	}
	m_componants.clear();
}

void GameManager::AddEntity(UINT entity, bool hasComponant, bool hasSystem)
{
	if (entity >= m_componants.size()) {
		m_componants.push_back(std::vector<Componant*>()); // Ajoute une nouvelle liste de composants pour cette entité
	} // Initialise la liste des composants pour cette entité

	if (hasComponant)
	{
		m_componants.push_back(std::vector<Componant*>()); // Ajoute une nouvelle liste de composants pour cette entité
	}
	if (hasSystem)
	{
		//m_systems.push_back(System);// Ajoute une nouvelle liste de systèmes pour cette entité
	}
}