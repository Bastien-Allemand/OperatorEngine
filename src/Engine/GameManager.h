#pragma once
#include <vector>
#include <unordered_map>
#include "Componant.h"

class GameManager
{
public:
	static GameManager* GetInstance();

	template<typename T>
	T AddComponant(UINT entity);

	template<typename C>
	C GetComponant(UINT entity);

private:
	static GameManager* m_instance;

	template<typename V>
	static std::vector<V>& GetComponantsArray()
	{
		static std::vector<V> m_componants;
		return m_componants;
	}

	std::unordered_map<UINT, std::vector<int>> m_componantIndexMap; //entity
};

template<typename T>
inline T GameManager::AddComponant(UINT entity)
{
	T componant;
	GetComponantsArray<T>().push_back(componant);
	m_componantIndexMap[entity].push_back(GetComponantsArray<T>().size() - 1);

	return componant;
}

template<typename C>
inline C GameManager::GetComponant(UINT entity)
{
	for (int index : m_componantIndexMap[entity])
	{
		auto& array = GetComponantsArray<C>();

		if (index >= 0 && index < array.size())
		{
			return array[index];
		}
	}
	return C();
}