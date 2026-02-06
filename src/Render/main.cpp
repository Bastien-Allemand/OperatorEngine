#include "pch.h"
#include "main.h"
#include "RenderDevice.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	const int width = 800;
	const int height = 600;
	const wchar_t* title = L"ENETRE";

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"SampleWindowClass";
	RegisterClass(&wc);

	RenderDevice renderDevice;
	return renderDevice.InitWindow(width, height, title, wc, hInstance, cmdShow);
}