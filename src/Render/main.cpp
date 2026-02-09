#include "pch.h"
#include "main.h"
#include "DDSTextureLoader12.h"
#include "RenderDevice.h"
#include "Window.h"
#include <iostream>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	Window window;
	window.Init(800, 600, L"ENETRE");

#if defined(DEBUG) | defined(_DEBUG)
	AllocConsole();
	FILE* pfile = nullptr;
	freopen_s(&pfile, "CONOUT$", "w", stdout);

	std::cout << "Console Initialized" << std::endl;
#endif
	RenderDevice renderDevice;
	renderDevice.Init(&window);

	renderDevice.Run();

#if defined(DEBUG) | defined(_DEBUG)
	fclose(stdout);
	FreeConsole();
#endif
    return 0;
}