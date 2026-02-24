#pragma once

#include <SDL_surface.h>
#include <SDL_video.h>
#include <string>

class Window
{
public:
    bool Init(const std::string& title, unsigned int width, unsigned int height);
    void Destroy();
    void UpdateInputs();

    bool ShouldClose() const;
    void SwapBuffers(SDL_Surface* buffer) const;
    void ResizeWindow(unsigned int width, unsigned int height);
    void GetWindowSize(unsigned int& width, unsigned int& height) const;

private:
    SDL_Surface* m_color_buffer = nullptr;
    unsigned int m_width = 0;
    unsigned int m_height = 0;
    bool m_should_close = false;

    SDL_Window* m_window = nullptr;
    static void PrintError(const std::string& message);
};
