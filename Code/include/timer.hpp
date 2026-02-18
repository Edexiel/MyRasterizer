#pragma once
#include <cstdint>

class Timer
{
public:
    // The various clock actions
    void Start();
    void Stop();
    void Pause();
    void Unpause();

    // Gets the timer's time
    uint32_t GetTicks() const;

    // Checks the status of the timer
    bool IsStarted() const;
    bool IsPaused() const;

private:
    // The clock time when the timer started
    uint32_t m_start_ticks = 0.f;

    // The ticks stored when the timer was paused
    uint32_t m_paused_ticks = 0.f;

    // The timer status
    bool m_paused = false;
    bool m_started = false;
};
