#pragma once

class Window
{
public:
	Window() = default;
	int Init(int _width, int _height, const wchar_t* _title);
	int GCwidth();
	int GCheight();
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	int m_width;
	int m_height;
};

