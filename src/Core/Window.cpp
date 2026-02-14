#include "pch.h"
#include "Window.h"

LRESULT CALLBACK Window::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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

bool Window::Init(int _width, int _height, const wchar_t* _title)
{
	m_width = _width;
	m_height = _height;

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = L"SampleWindowClass";
    wc.hbrBackground = NULL;
    RegisterClass(&wc);
    
    m_hwnd = CreateWindowEx(0,wc.lpszClassName,_title,
        WS_OVERLAPPEDWINDOW,CW_USEDEFAULT, CW_USEDEFAULT,
        _width, _height,nullptr, nullptr, 
        GetModuleHandle(nullptr), nullptr);
    if (!m_hwnd)
        return 1;

    int r = ShowWindow(m_hwnd, SW_SHOW);
	Log(r != 0, "Window was hidden");
    r = UpdateWindow(m_hwnd);
	Log(r == 0, "Updating Window");
    return 0;
}
int Window::GCWidth()
{
	return m_width;
}

int Window::GCHeight()
{
	return m_height;
}
