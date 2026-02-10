#include "pch.h"
#include "App.h"
#include "Window.h"
#include "RenderEngine.h"

LRESULT CALLBACK App::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void App::Initialize()
{
	{  //Console for debug
		#if defined(DEBUG) | defined(_DEBUG)
				AllocConsole();
				FILE* pfile = nullptr;
				freopen_s(&pfile, "CONOUT$", "w", stdout);

				std::cout << "Console Initialized" << std::endl;
		#endif
	}

	m_window = new Window;
	m_window->Init(800, 600, L"ENETRE", WndProc);

	m_renderEngine = new RenderEngine();
	m_renderEngine->Init(m_window->GCwidth(),m_window->GCheight(),m_window->GHWND());
	
}

App::~App()
{
#if defined(DEBUG) | defined(_DEBUG)
	fclose(stdout);
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

		}
	}
}
