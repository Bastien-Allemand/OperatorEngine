#pragma once
#include "System.h"
class MeshComponent;
class TransformComponent;
class Entity;

class MeshSystem
{
public:
	void AddEntity(Entity* entity);
	void Update();
private:
	Vector<Entity*> m_entities;
	UnOrderedMap<Entity*, MeshComponent*> m_meshComponents;
	UnOrderedMap<Entity*, TransformComponent*> m_TransformComponents;
};

