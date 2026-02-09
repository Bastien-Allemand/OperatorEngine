#pragma once

class Window
{
public:
	Window() = default;
	int Init(int _width, int _height, const wchar_t* _title, WNDPROC _WndProc);
	void Resize();
	int GCwidth();
	int GCheight();
	HWND GHWND() const { return m_hwnd; }
private:

	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;

	HWND m_hwnd = nullptr;
	int m_width = -1;
	int m_height = -1;
};

