#pragma once
#include <vector>
#include <unordered_map>
#include "Componant.h"
#include "Entity.h"
#include "System.h"
#include "ECS.h"
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

	Entity* AddEntity(UINT entity, bool hasComponant, bool hasSystem);

	//std::vector<System*> GetSystems() { return m_systems; }
	void Update();
	//ECS ecs;
private:
	float dt = 0.016f;
	static GameManager* m_instance;

	std::vector<std::vector<Componant*>> m_componants;
	std::vector<System*> m_systems;
	std::vector<Entity*> m_entities;

	std::unordered_map<UINT, Componant*> m_componentArrays;
	std::unordered_map<UINT, System*> m_systemArrays;

	// Helper pour récupérer le tableau typé
	//template<typename T>
	//std::shared_ptr<ComponentArray<T>> GetArray() {
	//	std::type_index typeName = std::type_index(typeid(T));
	//	// Si le tableau n'existe pas, on le crée
	//	if (m_componentArrays.find(typeName) == m_componentArrays.end()) {
	//		m_componentArrays[typeName] = std::make_shared<ComponentArray<T>>();
	//	}
	//	return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
	//}
};

template<typename T>
inline T& GameManager::AddComponant(UINT entity)
{
	for (Entity* comp : m_entities) {
		if (comp->id.first != entity) continue;
			T* newComponent = new T;
			comp->m_componants.push_back(new T);
		// Ajoute un nouveau composant de type T à l'entité
	}
	T* newComponent = new T; 
	newComponent->id = entity;

	m_componentArrays.insert({ entity, newComponent });

	m_componentArrays.find(entity)->second = newComponent;
	if (entity >= m_componants.size()) {
		throw std::out_of_range("Entité inconnue (indice hors bornes)");
	}
	m_componants[entity].push_back(newComponent);

	return *newComponent;
}

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

//template<typename C>
//inline C& GameManager::GetComponant(UINT entity)
//{
//	//auto& array = GetComponantsArray<C>();
//	for (auto index : m_componants[entity])
//	{
//
//		if (index >= 0 && index < array.size())
//		{
//			return array[index];
//		}
//	}
//	throw std::out_of_range("Composant non trouvé pour l'entité demandée");
//}

template<typename C>
inline C& GameManager::GetComponant(UINT entity)
{
	if (entity >= m_componants.size()) {
		throw std::out_of_range("Entité inconnue (indice hors bornes)");
	}

	// Parcourt les composants de l'entité et tente de caster vers C
	for (auto& basePtr : m_componants[entity]) {

		C* derived = dynamic_cast<C*>(basePtr);

		if (derived != nullptr) {
			return *derived;
		}
	}
	throw std::runtime_error("Composant de ce type non trouvé pour l'entité demandée");
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

template<typename S>
inline S& GameManager::AddSystem(UINT entity)
{
	S* newSystem = new S;
	newSystem->id = entity;
	//if (entity >= m_systems.size()) 
	//{
	//	throw std::out_of_range("Entité inconnue (indice hors bornes)");
	//}
	m_systems.push_back(newSystem);

	return *newSystem;
}

template<typename S>
inline S& GameManager::AddSystem(Entity* entity)
{
	for (auto& basePtr : m_systems) {

		S* derived = dynamic_cast<S*>(basePtr);

		if (derived != nullptr) {
			basePtr->m_entitiesss.push_back(entity);
			return *derived;
		}
	}
	S* newSystem = new S;
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

