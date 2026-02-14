#pragma once
class MeshSystem;
class Entity;
class ISystem;

class SystemManager
{
public:
	SystemManager();
	void Update();
private:
	Vector<ISystem*> m_systems;
};