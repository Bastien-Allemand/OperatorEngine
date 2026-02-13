#pragma once

class Window
{
public:
	Window() = default;
	bool Init(int _width, int _height, const wchar_t* _title, WNDPROC _WndProc);
	int GCWidth();
	int GCHeight();

	HWND GHWND() const { return m_hwnd; }
private:
	HWND m_hwnd = nullptr;
	int m_width = -1;
	int m_height = -1;
};

