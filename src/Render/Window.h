#pragma once

class Window
{
public:
	Window() = default;
	int Init(int _width, int _height, const wchar_t* _title);
	void Resize();
	int GCwidth();
	int GCheight();
	HWND GHWND() const { return m_hwnd; }
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;

	HWND m_hwnd;
	int m_width;
	int m_height;
};

