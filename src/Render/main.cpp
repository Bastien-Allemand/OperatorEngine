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
    while (true) {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            // 1. Record Commands (Clear screen, Draw Object)
            // 2. Execute Command List
            // 3. Present Swap Chain
            // 4. Wait for GPU (Signal Fence)
        }
    }
    return 0;
}