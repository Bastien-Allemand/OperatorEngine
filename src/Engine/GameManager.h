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

		std::vector<Componant> m_componants;
		std::unordered_map<UINT, std::vector<int>> m_componantIndexMap; //entity
};

template<typename T>
inline T GameManager::AddComponant(UINT entity)
{
	T componant;
	m_componants.push_back(componant);
	m_componantIndexMap[entity].push_back(m_componants.size() - 1);

	return componant;
}

template<typename C>
inline C GameManager::GetComponant(UINT entity)
{
	for (int index : m_componantIndexMap[entity])
	{
		if (std::is_same<C, std::remove_cvref_t<decltype(m_componants[index])>>::value)
		{
			return static_cast<C>(m_componants[index]);
		}
	}
}