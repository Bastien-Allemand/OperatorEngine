#include "pch.h"
#include "Entity.h"

void Entity::AddComponant(Componant* componant)
{
	componant->id = id.first; // Associe l'ID de l'entité au composant
	m_componants.push_back(componant);
}

void Entity::AddChild(Entity* child)
{
	if (child == this) return;
	m_children.push_back(child); 
	child->m_parent = this; 
}
