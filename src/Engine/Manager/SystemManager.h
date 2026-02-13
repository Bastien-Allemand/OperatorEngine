#pragma once
class System;

class SystemManager
{
public:
	bool AddSystem(System* _system);
	void Update();
private:
	Vector<System*> m_systems;
};