#pragma once
class Window;
class RenderEngine;

class App
{
public:
	App() = default;
	void Initialize(int _width, int _height);
	~App();

	void Run();
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window* m_window = nullptr;

	RenderEngine* m_renderEngine = nullptr;


};

