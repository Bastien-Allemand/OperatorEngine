#include "pch.h"
#include "App.h"
#include "RenderEngine.h"
#include <conio.h>
#include "Geometry.h"
#include "Mesh.h"
#include "Core/Window.h"

void App::Initialize(int _width,int _height)
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
	r = m_window->Init(_width, _height, L"ENETRE");
	Log(r, "Initializing Window");
	m_renderEngine = new RenderEngine();
	r = m_renderEngine->Init(m_window->GCWidth(), m_window->GCHeight(), m_window->GHWND());
	Log(r,"Initializing RenderEngine");
	
	m_lastTime = clock::now();

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

App::~App()
{
#if defined(DEBUG) | defined(_DEBUG)
	fclose(stdout);
	fclose(stderr);
	FreeConsole();
#endif
}

void App::Run()
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
			m_renderEngine->Draw(m_mesh,DirectX::XMLoadFloat4x4(&m_obj1));
			m_renderEngine->Draw(m_mesh,DirectX::XMLoadFloat4x4(&m_obj2));
			m_renderEngine->CloseDraw();
		}
	}
}
