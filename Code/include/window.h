#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "Color.hpp"

struct Pixel
{
	int x = 0;
	int y = 0;

	Pixel(int x, int y);
};

class Window
{
private:
	unsigned int width = 0;
	unsigned int height = 0;
	bool shouldClose = false;

	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;

	SDL_Event events{};

	static void PrintError(const std::string& message);

public:
	bool Init(const std::string& title, unsigned int width, unsigned int height);
	void Destroy();
	void UpdateInputs();
	// void BeginDrawing();
	// void EndDrawing();
	// void ClearBackground(const Color& color);
	// void DrawPixel(const Pixel& pixel, const Color& color);

	bool ShouldClose() const;
	void SwapBuffers(SDL_Surface* buffer);
};
