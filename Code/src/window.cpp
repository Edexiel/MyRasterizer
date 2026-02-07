#include "window.h"
#include <iostream>

Pixel::Pixel(int _x, int _y) : x(_x), y(_y) {}

bool Window::Init(const std::string& title, unsigned int _width, unsigned int _height)
{
    width = _width;
    height = _height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        PrintError("SDL initialization failed.");
        return false;
    }

    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                              SDL_WINDOW_SHOWN);

    if (window == nullptr)
    {
        PrintError("Window creation failed.");
        Destroy();
        return false;
    }

    screenSurface = SDL_GetWindowSurface(window);

    //
    // renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // if (renderer == nullptr)
    // {
    //     PrintError("Renderer creation failed.");
    //     Destroy();
    //     return false;
    // }

    return true;
}

void Window::Destroy()
{
    if (window)
    {
        SDL_DestroyWindow(window);
    }

    SDL_Quit();
}

// void Window::BeginDrawing()
// {
//     UpdateInputs();
// }
//
// void Window::EndDrawing()
// {
//     SDL_RenderPresent(renderer);
// }

//
// void Window::DrawPixel(const Pixel& pixel, const Color& color)
// {
//     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
//     SDL_RenderDrawPoint(renderer, pixel.x, pixel.y);
// }

bool Window::ShouldClose() const { return shouldClose; }

void Window::SwapBuffers(SDL_Surface* buffer)
{
    SDL_Rect rect{0, 0, (int)width, (int)height};
    SDL_BlitSurface(buffer, &rect, SDL_GetWindowSurface(window), &rect);

    SDL_UpdateWindowSurface(window);
}

void Window::PrintError(const std::string& message)
{
    std::cout << message << " SDL_Error: " << SDL_GetError() << std::endl;
}

void Window::UpdateInputs()
{
    SDL_PollEvent(&events);

    switch (events.type)
    {
    case SDL_QUIT:
        shouldClose = true;
        break;
    }
}
