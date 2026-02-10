#include "pch.h"
#include "ECS.h"

//ECS::ECS()
//{
//	m_instance = GameManager::GetInstance();
//}
//
//ECS::~ECS()
//{
//	for (auto& entityComponents : m_componants) {
//		for (auto* comp : entityComponents) {
//			delete comp; // Supprime réellement l'objet en mémoire
//		}
//		entityComponents.clear();
//	}
//	m_componants.clear();
//}
//
//Entity& ECS::CreateEntity()
//{
//	static UINT nextEntityId = 0;
//	Entity newEntity;
//	newEntity.id = std::make_pair(nextEntityId++, 1); // Génère un nouvel ID unique
//	AddEntity(newEntity.id.first); // Ajoute l'entité au système
//	m_componants.push_back(std::vector<Componant*>()); // Prépare un espace pour les composants de cette entité
//	return newEntity;
//}
