#pragma once
#include <vector>
#include <unordered_map>
#include "Componant.h"
#include "System.h"
#include <typeindex> // Nécessaire pour identifier les types T
#include <memory>
#include <stdexcept>
#include <iostream>

class GameManager
{
public:
	static GameManager* GetInstance();

	~GameManager() {
		for (auto& entityComponents : m_componants) {
			for (auto* comp : entityComponents) {
				delete comp; // Supprime réellement l'objet en mémoire
			}
			entityComponents.clear();
		}
		m_componants.clear();
	}

	template<typename T>
	T& AddComponant(UINT entity);

	template<typename C>
	C& GetComponant(UINT entity);

	void AddEntity(UINT entity) {
		if (entity >= m_componants.size()) {
			m_componants.resize(entity + 1);
		}
	}

private:
	static GameManager* m_instance;

	std::vector<std::vector<Componant*>> m_componants; //componant

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

