#pragma once

#include "ComponentPool.h"

class ComponentManager
{
public:

	template<typename T>
	ComponentPool<T>* GPool();

	template<typename T>
	T* AddComponent(uint32 entityId);

	template<typename T>
	T* GComponent(uint32 entityId);



	template<typename T>
	void RemoveComponent(uint32 entityId);
	/// <summary>
	/// removes all the components of the entity from all the pools
	/// </summary>
	/// <param name="_entityId"></param>
	void RemoveEntity(uint32 _entityId);

private:
	Vector<IComponentPool*> m_componentPools;
	Vector<size_t> m_poolId;

	Vector<uint32> m_entityIds;
};

template<typename T>
inline ComponentPool<T>* ComponentManager::GPool()
{
	size_t typeId = typeid(T).hash_code();
	auto it = std::find(m_poolId.begin(), m_poolId.end(), typeId);
	if (it == m_poolId.end())
	{
		ComponentPool<T>* newPool = new ComponentPool<T>();

		m_componentPools.push_back(newPool);
		m_poolId.push_back(typeId);

		return static_cast<ComponentPool<T>*>(m_componentPools.back());
	}
	else
	{
		size_t index = std::distance(m_poolId.begin(), it);

		return static_cast<ComponentPool<T>*>(m_componentPools[index]);
	}
}

template<typename T>
inline T* ComponentManager::AddComponent(uint32 entityId)
{
	ComponentPool<T>* pool = GPool<T>();
	//adds Empty component to the pool and returns the pointer to it
	pool->Add(entityId, T());
	return pool->GComponent(entityId);
}

template<typename T>
inline T* ComponentManager::GComponent(uint32 entityId)
{
	ComponentPool<T>* pool = GPool<T>();
	return pool->GComponent(entityId);
}

template<typename T>
inline void ComponentManager::RemoveComponent(uint32 entityId)
{
	ComponentPool<T>* pool = GPool<T>();
	pool->Remove(entityId);
}
