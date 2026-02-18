#include "timer.hpp"

#include <SDL_timer.h>

void Timer::Start()
{
    //Start the timer
    m_started = true;

    //Unpause the timer
    m_paused = false;

    //Get the current clock time
    m_start_ticks = SDL_GetTicks();
    m_paused_ticks = 0;
}

void Timer::Stop()
{
    //Stop the timer
    m_started = false;

    //Unpause the timer
    m_paused = false;

    //Clear tick variables
    m_start_ticks = 0;
    m_paused_ticks = 0;
}

void Timer::Pause()
{
    //If the timer is running and isn't already paused
    if (m_started && !m_paused)
    {
        //Pause the timer
        m_paused = true;

        //Calculate the paused ticks
        m_paused_ticks = SDL_GetTicks() - m_start_ticks;
        m_start_ticks = 0;
    }
}

void Timer::Unpause()
{
    //If the timer is running and paused
    if (m_started && m_paused)
    {
        //Unpause the timer
        m_paused = false;

        //Reset the starting ticks
        m_start_ticks = SDL_GetTicks() - m_paused_ticks;

        //Reset the paused ticks
        m_paused_ticks = 0;
    }
}

uint32_t Timer::GetTicks() const
{
    //The actual timer time
    uint32_t time = 0;

    //If the timer is running
    if (m_started)
    {
        //If the timer is paused
        if (m_paused)
        {
            //Return the number of ticks when the timer was paused
            time = m_paused_ticks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - m_start_ticks;
        }
    }

    return time;
}

bool Timer::IsStarted() const
{
    //Timer is running and paused or unpaused
    return m_started;
}

bool Timer::IsPaused() const
{
    //Timer is running and paused
    return m_paused && m_started;
}
