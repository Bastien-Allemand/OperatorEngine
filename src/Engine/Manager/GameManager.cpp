#include "pch.h"
#include "GameManager.h"
#include "SystemManager.h"
#include "Core/Window.h"
#include "Render/RenderEngine.h"
#include "Render/Geometry.h"
#include "Render/Mesh.h"

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
	r = m_window->Init(1980, 1080, L"ENETRE");
	Log(r, "Initializing Window");
	m_renderEngine = new RenderEngine();
	r = m_renderEngine->Init(m_window->GCWidth(), m_window->GCHeight(), m_window->GHWND());
	Log(r, "Initializing RenderEngine");

	m_systemManager = new SystemManager();

	Geometry geo;
	geo.BuildBox();
	m_mesh = new Mesh(geo);
	m_renderEngine->InitMesh(m_mesh);
	Matrix world = DirectX::XMMatrixIdentity();
	//Vector3f pos = { 0,0,4 };
	//world = world * DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos));
	DirectX::XMStoreFloat4x4(&m_obj1, world);
	Vector3f pos2 = { 2,0,0 };
	world = DirectX::XMMatrixIdentity();
	world = world * DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&pos2));

	DirectX::XMStoreFloat4x4(&m_obj2, world);
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
			auto currentTime = clock::now();
			std::chrono::duration<float> deltaTime = currentTime - m_lastTime;
			m_lastTime = currentTime;
			if (m_timer > 0)
			{
				m_timer -= deltaTime.count();
				fps += 1;
			}
			else
			{
				WString fpsString = L"ENETRE | FPS:" + std::to_wstring((int)fps);
				// 3. Mettre à jour le titre de la fenêtre
				SetWindowText(m_window->GHWND(), fpsString.c_str());
				m_timer = 1;
				fps = 0;
			}

			m_renderEngine->Update(deltaTime.count());
			m_renderEngine->BeginDraw();
			m_renderEngine->Draw(m_mesh, DirectX::XMLoadFloat4x4(&m_obj1));
			m_renderEngine->Draw(m_mesh, DirectX::XMLoadFloat4x4(&m_obj2));
			m_renderEngine->CloseDraw();
		}
	}
}


Entity* GameManager::GetEntity(Id _entity)
{
	for (auto entite : m_entities)
		if (entite->id == _entity)
			return entite;
}

Id* GameManager::AddEntity()
{
	Entity* newEntity = new Entity();
	newEntity->id.index = m_entities.size();
	m_entities.push_back(newEntity);
	return &newEntity->id;
}

void GameManager::Update()
{
	m_systemManager->Update();
}