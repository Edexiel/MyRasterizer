#include "window.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_rect.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <iostream>
#include <string>

bool Window::Init(const std::string& title, unsigned int width, unsigned int height)
{
    m_width = width;
    m_height = height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        PrintError("SDL initialization failed.");
        return false;
    }

    m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_width, m_height, SDL_WINDOW_SHOWN);

    if (m_window == nullptr)
    {
        PrintError("Window creation failed.");
        Destroy();
        return false;
    }


    return true;
}

void Window::Destroy()
{
    if (m_window)
    {
        SDL_DestroyWindow(m_window);
    }

    SDL_Quit();
}

bool Window::ShouldClose() const
{
    return m_should_close;
}

void Window::SwapBuffers(SDL_Surface* buffer) const
{
    SDL_Rect rect{0, 0, (int) m_width, (int) m_height};
    SDL_BlitSurface(buffer, &rect, SDL_GetWindowSurface(m_window), &rect);

    SDL_UpdateWindowSurface(m_window);
}

void Window::ResizeWindow(unsigned int width, unsigned int height)
{
    if (m_width == width && m_height == height)
    {
        return;
    }

    m_width = width;
    m_height = height;

    if (m_draw_surface)
        SDL_FreeSurface(m_draw_surface);
}

void Window::GetWindowSize(unsigned int& width, unsigned int& height) const
{
    width = m_width;
    height = m_height;
}

void Window::PrintError(const std::string& message)
{
    std::cout << message << " SDL_Error: " << SDL_GetError() << '\n';
}

void Window::UpdateInputs()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
                m_should_close = true;
                break;
            case SDL_WINDOWEVENT:
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                        ResizeWindow(event.window.data1,event.window.data2);
                        break;
                }
                break;
        }
    }
}