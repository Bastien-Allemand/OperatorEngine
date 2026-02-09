#include "pch.h"
#include "main.h"
#include "App.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
	App app;
	app.Initialize();
	app.Run();

    return 0;
}