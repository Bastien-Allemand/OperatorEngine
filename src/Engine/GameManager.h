#pragma once
#include <vector>
#include <unordered_map>
#include "Componant.h"
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

	template<typename C>
	C& GetComponant(UINT entity);

	template<typename S>
	S& AddSystem();

	template<typename C>
	C& GetSystem();

	void AddEntity(UINT entity);

	std::vector<System*> GetSystems() { return m_systems; }

private:

	static GameManager* m_instance;

	std::vector<std::vector<Componant*>> m_componants;
	std::vector<System*> m_systems;



	std::unordered_map<std::type_index, std::shared_ptr<Componant>> m_componentArrays;

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

//template<typename T>
//inline T& GameManager::AddComponant(UINT entity)
//{
//	T componant;
//	m_componants[entity].push_back(componant);
//	m_componantIndexMap[entity].push_back(GetComponantsArray<T>().size() - 1);
//	return componant;
//}

template<typename T>
inline T& GameManager::AddComponant(UINT entity)
{
	T* newComponent = new T; 
	newComponent->id = entity;
	if (entity >= m_componants.size()) {
		throw std::out_of_range("Entité inconnue (indice hors bornes)");
	}
	m_componants[entity].push_back(newComponent);

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

template<typename S>
inline S& GameManager::AddSystem()
{
	S* newSystem = new S;
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

