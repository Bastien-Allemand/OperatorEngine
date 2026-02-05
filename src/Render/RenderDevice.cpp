#include "pch.h"
#include "RenderDevice.h"

RenderDevice::RenderDevice()
{

}

int RenderDevice::InitWindow(int _width, int _height,const wchar_t* _title, WNDCLASS _windowsClass, HINSTANCE _hInstance, int _cmdShow)
{
    HWND hwnd = CreateWindowEx(
        0,
        _windowsClass.lpszClassName,
        L"ENETRE",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, _width, _height,
        nullptr, nullptr, _hInstance, nullptr);
    if (!hwnd)
        return 0;
    ShowWindow(hwnd, _cmdShow);
    UpdateWindow(hwnd);
    MSG msg = { 0 };
    while (GetMessage(&msg, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
	return static_cast<int>(msg.wParam);
}
