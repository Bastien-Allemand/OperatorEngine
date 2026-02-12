#include "pch.h"
#include "Entity.h"

void Entity::AddComponant(Componant* componant)
{
	componant->id = id.first; // Associe l'ID de l'entité au composant
	m_componants.push_back(componant);
}
