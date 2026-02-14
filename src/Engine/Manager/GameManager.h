#pragma once
#include <chrono>

class SystemManager;
class ComponentManager;
class Window;
class RenderEngine;
class Mesh;



class GameManager
{
public:
	static GameManager* GetInstance();
	~GameManager();

	void Run();

	ComponentManager* GComponentManager() { return m_componentManager; }
	uint32 GNewEntityId();
	void DestroyEntity(uint32 _entityId);
	RenderEngine* GRenderEngine() { return m_renderEngine; }
	Window* GWindow() { return m_window; }
	float GFps() { return m_fps; }
	float GDeltaTime() { return m_deltaTime; }

private:
	GameManager();
	static GameManager* m_instance;
	std::chrono::steady_clock::time_point m_lastTime;
	float m_timer = 1;
	float m_fps = 0;
	float m_deltaTime = 0;

	Vector<uint32> m_entityIds;
	Vector<uint32> m_freeEntityIds;
	
	SystemManager* m_systemManager;
	ComponentManager* m_componentManager;

	Window* m_window;
	RenderEngine* m_renderEngine;

};

