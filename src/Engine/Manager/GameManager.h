#pragma once

#include "Entity.h"
#include "Component.h"
#include <chrono>
class SystemManager;
class Window;
class RenderEngine;
class Mesh;

class GameManager
{
public:
	static GameManager* GetInstance();
	float GetDeltaTime() {}
	~GameManager();

	void Run();

	template<typename T>
	T& AddComponent(Id _entity);

	template<typename C>
	C& GetComponent(Id _entity);

	Entity* GetEntity(Id _entity);

	Id* AddEntity();

private:
	using clock = std::chrono::steady_clock; // monotonic clock, no jumps
	std::chrono::steady_clock::time_point m_lastTime;
	float m_timer = 1;
	float fps = 0;
	float deltaTime = 0;
	Mesh* m_mesh = nullptr;
	Matrix4x4f m_obj1;
	Matrix4x4f m_obj2;


	void Update();
	GameManager();
	Window* m_window;
	RenderEngine* m_renderEngine;
	static GameManager* m_instance;

	SystemManager* m_systemManager;

	std::vector<Entity*> m_entities;
};

template<typename T>
inline T& GameManager::AddComponent(Id _entity)
{
	Entity* entity = GetEntity(_entity);
	T* newComponent = new T();
	entity->componants.push_back(newComponent);
	return *newComponent;
}

template<typename C>
inline C& GameManager::GetComponent(Id _entity)
{

}

