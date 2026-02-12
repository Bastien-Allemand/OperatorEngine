#include "pch.h"
#include "App.h"
#include "Window.h"
#include "RenderEngine.h"

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
	{  //Console for debug
		#if defined(DEBUG) | defined(_DEBUG)
				AllocConsole();
				FILE* pfile = nullptr;
				freopen_s(&pfile, "CONOUT$", "w", stdout);
				freopen_s(&pfile, "CONOUT$", "w", stderr);

				std::cout << "Console Initialized" << std::endl;
		#endif
	}
	m_window = new Window;
	m_window->Init(_width, _height, L"ENETRE", WndProc);

	m_renderEngine = new RenderEngine();
	m_renderEngine->Init(m_window->GCWidth(),m_window->GCHeight(),m_window->GHWND());
	
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
			// 1. Calculer le FPS float fps = 1.0f / deltaTime.count();
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
			m_renderEngine->Draw();
		}
	}
}
