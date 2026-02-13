#pragma once

#include "Entity.h"
#include "Component.h"

class SystemManager;

class GameManager
{
public:
	static GameManager* GetInstance();

	~GameManager();

	template<typename T>
	T& AddComponent(Id _entity);

	template<typename C>
	C& GetComponent(Id _entity);

	Entity* GetEntity(Id _entity);

	Id* AddEntity();

	void Update();
private:

	static GameManager* m_instance;
	SystemManager* m_systemManager;
	std::vector<Entity*> m_entities;
};

template<typename T>
inline T& GameManager::AddComponent(Id _entity)
{
	Entity* entity = GetEntity(_entity);
	T* newComponent = new T();
	entity->componants.push_back(newComponent);
	return *newComponent;
}

template<typename C>
inline C& GameManager::GetComponent(Id _entity)
{
	for (auto& basePtr : entity->m_componants) {

		C* derived = dynamic_cast<C*>(basePtr);

		if (derived != nullptr) {
			return *derived;
		}
	}
}

