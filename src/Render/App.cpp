#include "pch.h"
#include "App.h"
#include "Window.h"
#include "RenderEngine.h"
#include <conio.h>
#include "Geometry.h"
#include "Mesh.h"

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void App::Initialize(int _width,int _height)
{
	#if defined(DEBUG) | defined(_DEBUG)
		bool r = AllocConsole();
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
	r = m_window->Init(_width, _height, L"ENETRE", WndProc);
	Log(r, "Initializing Window");
	m_renderEngine = new RenderEngine();
	r = m_renderEngine->Init(m_window->GCWidth(), m_window->GCHeight(), m_window->GHWND());
	Log(r,"Initializing RenderEngine");
	
	m_lastTime = clock::now();
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
				if (!test)
				{
					Geometry geo;
					geo.BuildBox();
					m_mesh = new Mesh(geo);
					m_renderEngine->AddMeshToDraw(m_mesh); Log(false, "Adding Box Mesh to Draw List");
					test = true;
				}
			}

			m_renderEngine->Update(deltaTime.count());
			m_renderEngine->Draw();
		}
	}
}
