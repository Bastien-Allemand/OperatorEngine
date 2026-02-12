#pragma once
#include <iostream>
#include <vector>
#include "Componant.h"



#define INDEX UINT
#define NUMBER_OF_COMPONENTS int
class Entity
{
public:
	std::pair<INDEX, NUMBER_OF_COMPONENTS> id;
	INDEX ids;

	std::vector<Componant*> m_componants;
	std::vector<Entity*> m_children;
	Entity* m_parent;
	void AddComponant(Componant* componant);
	void AddChild(Entity* child);
};