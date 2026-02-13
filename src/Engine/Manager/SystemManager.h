#pragma once
class MeshSystem;
class Entity;

class SystemManager
{
public:
	void FilterEntity(Entity* _entity);
	void Update();
private:
	MeshSystem* m_meshSystem;
};