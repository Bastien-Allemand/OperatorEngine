#pragma once
#include <vector>
#include <unordered_map>
#include "Componant.h"
#include "Entity.h"
#include "System.h"
#include <typeindex> // Nécessaire pour identifier les types T
#include <memory>
#include <stdexcept>
#include <iostream>

class System;

class GameManager
{
public:
	static GameManager* GetInstance();

	~GameManager();

	template<typename T>
	T& AddComponant(UINT entity);

	template<typename T>
	T& AddComponant(Entity* entity);

	template<typename C>
	C& GetComponant(UINT entity);

	template<typename C>
	C& GetComponant(Entity* entity);

	template<typename S>
	S& AddSystem(UINT entity);

	template<typename S>
	S& AddSystem(Entity* entity);

	template<typename C>
	C& GetSystem();

	Entity* AddEntity(UINT entity);

	void Update();
private:
	float dt = 0.016f;
	static GameManager* m_instance;

	std::vector<std::vector<Componant*>> m_componants;
	std::vector<System*> m_systems;
	std::vector<Entity*> m_entities;

	std::unordered_map<UINT, Componant*> m_componentArrays;
	std::unordered_map<UINT, System*> m_systemArrays;
};



template<typename T>
inline T& GameManager::AddComponant(Entity* entity)
{
	T* newComponent = new T; // Ajoute un nouveau composant de type T à l'entité
	newComponent->id = entity->id.first;

	/*if (entity >= m_componants.size()) {
		throw std::out_of_range("Entité inconnue (indice hors bornes)");
	}*/
	entity->m_componants.push_back(newComponent);

	return *newComponent;
}

template<typename C>
inline C& GameManager::GetComponant(Entity* entity)
{
	//if (entity >= m_componants.size()) {
	//	throw std::out_of_range("Entité inconnue (indice hors bornes)");
	//}

	// Parcourt les composants de l'entité et tente de caster vers C
	for (auto& basePtr : entity->m_componants) {

		C* derived = dynamic_cast<C*>(basePtr);

		if (derived != nullptr) {
			return *derived;
		}
	}
	throw std::runtime_error("Composant de ce type non trouvé pour l'entité demandée");
}

template<typename NewSystem>
inline NewSystem& GameManager::AddSystem(Entity* entity)
{
	for (auto& basePtr : m_systems) {

		NewSystem* derived = dynamic_cast<NewSystem*>(basePtr);

		if (derived != nullptr) {
			basePtr->m_entitiesss.push_back(entity);
			return *derived;
		}
	}
	NewSystem* newSystem = new NewSystem;
	newSystem->m_entitiesss.push_back(entity);
	m_systems.push_back(newSystem);
	return *newSystem;
}

template<typename C>
inline C& GameManager::GetSystem()
{
	// Parcourt les composants de l'entité et tente de caster vers C
	for (auto& basePtr : m_systems) {
		C* derived = dynamic_cast<C*>(basePtr);

		if (derived != nullptr) {
			return *derived;
		}
	}
	throw std::runtime_error("Composant de ce type non trouvé pour l'entité demandée");
}

