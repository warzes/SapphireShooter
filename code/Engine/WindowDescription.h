#pragma once

struct WindowDescription
{
	int width = 1024;
	int height = 768;

	bool fullscreen = false;
	bool resizable = false;

	std::wstring Title = L"Game";
};