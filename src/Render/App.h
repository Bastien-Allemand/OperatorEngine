#pragma once
class Window;
class RenderEngine;
class Mesh;
#include <chrono>
class App
{
public:
	App() = default;
	void Initialize(int _width, int _height);
	~App();
	using clock = std::chrono::steady_clock; // monotonic clock, no jumps
	std::chrono::steady_clock::time_point m_lastTime;
	void Run();
	float m_timer = 1;
	float fps = 0;
private:
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	Window* m_window = nullptr;
	Mesh* m_mesh = nullptr;
	RenderEngine* m_renderEngine = nullptr;
	Matrix4x4f m_obj1;
	Matrix4x4f m_obj2;
	bool test = false;
};

