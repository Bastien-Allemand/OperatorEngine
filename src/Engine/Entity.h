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
	void AddComponant(Componant* componant);

};