#include "pch.h"
#include "main.h"
#include "DDSTextureLoader12.h"
#include "RenderDevice.h"
#include "Window.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	Window window;
	window.Init(800, 600, L"ENETRE");

	RenderDevice renderDevice;
	renderDevice.Init();
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}