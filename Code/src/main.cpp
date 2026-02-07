#include "Rasterizer.hpp"
#include "window.h"

#include "Camera.hpp"
#include "InputManager.hpp"
#include <Timer.hpp>

#include <iostream>

#include <SDL.h>

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

int main(int argc, char** argv)
{
    Window window;
    if (!window.Init("Rasterizer", SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        return EXIT_FAILURE;
    }
    //
    // // Time && fps
    // double time = 0.f;
    // double deltaTime = 0.01;
    // const float sample = 1.f; // moyenne de fps sur sample seconde(s)
    // uint frames = 0;
    // double time_acc = 0.f;
    // std::string fps;

    InputManager inputManager;
    Camera camera{inputManager, Vec3::Zero(),Vec3::Zero(), 0.f, 0.f, 0.5f, 1.f};
    Scene scene{inputManager, camera};

    Rasterizer rasterizer{SCREEN_WIDTH, SCREEN_HEIGHT};

    // Keeps track of time between steps

    Timer applicationTimer;
    Timer stepTimer;

    applicationTimer.Start();

    std::string fps;

    float time_acc = 0.f;
    float sample_time = 1.f;
    int frames = 0;

    while (!window.ShouldClose())
    {
        const float timeStep = stepTimer.GetTicks() / 1000.f;

        // Compute FPS
        {
            time_acc += timeStep;
            frames++;
            if (time_acc >= sample_time)
            {
                std::cout << "FPS: " << 1.f / (time_acc / frames) << std::endl;
                frames = 0;
                time_acc = 0;
            }
        }

        // window.DrawPixel(Pixel(10, 10), Color{0, 0, 0});

        // poll events


        // Updates
        window.UpdateInputs();
        // inputManager.Update();
        // camera.Update(timeStep);
        scene.Update(timeStep);

        // reset timer
        stepTimer.Start();

        // Drawing
        rasterizer.RenderScene(scene);

        window.SwapBuffers(rasterizer.GetColorBuffer());
    }

    window.Destroy();
    return EXIT_SUCCESS;
}
