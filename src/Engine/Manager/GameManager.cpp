#include "pch.h"
#include "GameManager.h"
#include "SystemManager.h"
#include "ComponentManager.h"

#include "Core/Window.h"
#include "Render/RenderEngine.h"


GameManager* GameManager::m_instance = nullptr;

GameManager* GameManager::GetInstance()
{
	return (m_instance == nullptr) ? m_instance = new GameManager() : m_instance;
}

GameManager::GameManager()
{
	bool r;
#if defined(DEBUG) | defined(_DEBUG)
	r = AllocConsole();
	FILE* pfile = nullptr;
	freopen_s(&pfile, "CONOUT$", "w", stdout);
	freopen_s(&pfile, "CONOUT$", "w", stderr);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	bool r2 = GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
	Log(!r && !r2, "Console Initializing");
#endif
	m_window = new Window;
	r = m_window->Init(990, 540, L"ENETRE");
	Log(r, "Initializing Window");
	m_renderEngine = new RenderEngine();
	r = m_renderEngine->Init(m_window->GCWidth(), m_window->GCHeight(), m_window->GHWND());
	Log(r, "Initializing RenderEngine");

	m_systemManager = new SystemManager();
	m_componentManager = new ComponentManager();
}
GameManager::~GameManager()
{
#if defined(DEBUG) | defined(_DEBUG)
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
#endif
}

void GameManager::Run()
{
	MSG msg = { 0 };
	while (true) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			auto currentTime = std::chrono::steady_clock::now();
			std::chrono::duration<float> deltaTime = currentTime - m_lastTime;
			m_deltaTime = deltaTime.count();
			m_lastTime = currentTime;
			if (m_timer > 0)
			{
				m_timer -= deltaTime.count();
				m_fps += 1;
			}
			else
			{
				WString fpsString = L"ENETRE | FPS:" + std::to_wstring((int)m_fps);
				// 3. Mettre à jour le titre de la fenêtre
				SetWindowText(m_window->GHWND(), fpsString.c_str());
				m_timer = 1;
				m_fps = 0;
			}

			m_renderEngine->Update(deltaTime.count());
			m_systemManager->Update();
		}
	}
}

uint32 GameManager::GNewEntityId()
{
	uint32 newId;
	if (m_freeEntityIds.size() > 0)
	{
		newId = m_freeEntityIds[0];
		m_freeEntityIds.erase(m_freeEntityIds.begin());
		m_entityIds[newId] = newId;
	}
	newId = m_entityIds.size();
	m_entityIds.push_back(newId);
	return newId;
}

void GameManager::DestroyEntity(uint32 _entityId)
{
	m_entityIds[_entityId] = -1;
	m_freeEntityIds.push_back(_entityId);

	m_componentManager->RemoveEntity(_entityId);
}
