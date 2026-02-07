#pragma once
#include <cstdint>

class Timer
{
public:
    // Initializes variables
    Timer();

    // The various clock actions
    void Start();
    void Stop();
    void Pause();
    void Unpause();

    // Gets the timer's time
    uint32_t GetTicks();

    // Checks the status of the timer
    bool IsStarted();
    bool IsPaused();

private:
    // The clock time when the timer started
    uint32_t mStartTicks;

    // The ticks stored when the timer was paused
    uint32_t mPausedTicks;

    // The timer status
    bool mPaused;
    bool mStarted;
};
