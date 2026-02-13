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
	~GameManager();

	void Run();

	template<typename C>
	C& GComponent(Id _entity);

	Entity* GEntity(Id _entity);

	void AddEntity(Entity _entity);

	RenderEngine* GetRenderEngine() { return m_renderEngine; }
	Window* GetWindow() { return m_window; }
	float GetFPS() { return m_fps; }
	float GetDeltaTime() { return m_deltaTime; }

private:
	using clock = std::chrono::steady_clock; // monotonic clock, no jumps
	std::chrono::steady_clock::time_point m_lastTime;
	float m_timer = 1;
	float m_fps = 0;
	float m_deltaTime = 0;
	Mesh* m_mesh = nullptr;
	Matrix4x4f m_obj1;
	Matrix4x4f m_obj2;


	void Update();
	GameManager();
	Window* m_window;
	RenderEngine* m_renderEngine;
	static GameManager* m_instance;

	SystemManager* m_systemManager;

	Vector<Entity> m_entities;
};

template<typename C>
inline C& GameManager::GComponent(Id _entity)
{

}

