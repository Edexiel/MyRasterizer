#include <cstdlib>
#include <iostream>
#include <timer.hpp>

#include "camera.hpp"
#include "inputManager.hpp"
#include "rasterizer.hpp"
#include "scene.hpp"
#include "vec3.hpp"
#include "window.h"

constexpr int screenWidth = 1280;
constexpr int screenHeight =  720;

int main(int /*argc*/, char** /*argv*/)
{
    Window window;
    if (!window.Init("Rasterizer", screenWidth, screenHeight))
    {
        return EXIT_FAILURE;
    }

    InputManager inputManager;
    Camera camera{inputManager, Vec3::Zero(), Vec3::Zero(), 0.f, 0.f, 0.5f, 1.f};
    Scene scene{inputManager, camera};

    Rasterizer rasterizer{screenWidth, screenHeight};

    Timer applicationTimer;
    Timer stepTimer;

    applicationTimer.Start();

    // config timer
    float timeAcc = 0.f;
    constexpr float sampleTime = 1.f;
    int frames = 0;

    while (!window.ShouldClose())
    {
        const float timeStep = (float)stepTimer.GetTicks() / 1000.f;

        // Compute FPS
        {
            timeAcc += timeStep;
            frames++;
            if (timeAcc >= sampleTime)
            {
                std::cout << "FPS: " << 1.f / (timeAcc / (float)frames) << '\n';
                frames = 0;
                timeAcc = 0;
            }
        }

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