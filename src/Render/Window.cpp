#include "pch.h"
#include "Window.h"



bool Window::Init(int _width, int _height, const wchar_t* _title, WNDPROC _WndProc)
{
	m_width = _width;
	m_height = _height;

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = _WndProc;
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

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
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
