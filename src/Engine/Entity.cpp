#include "pch.h"
#include "Entity.h"

void Entity::AddComponant(Componant* componant)
{
	m_componants.push_back(componant);
}
