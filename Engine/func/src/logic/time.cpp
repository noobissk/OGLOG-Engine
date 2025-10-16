#include <logic/time.h>
#include <chrono>
#include <cmath>


float Time::delta = 0.0f;
float Time::fixed_delta = 0.0f;
float Time::smooth_delta = 0.0f;
float Time::sine = 0.0f;
float Time::cosine = 0.0f;


using namespace std::chrono;

void Time::update()
{
    static auto _time_old = high_resolution_clock::now();
    auto _time = high_resolution_clock::now();

    duration<float> frameTime = _time - _time_old;
    _time_old = _time;

    delta = frameTime.count();

    time += delta;
    smooth_delta += (delta - smooth_delta) * m_smooth_delta_factor;
    sine = std::sinf(time);
    cosine = std::cosf(time);
}

void Time::updateFixed()
{
    static auto _time_old = high_resolution_clock::now();
    auto _time = high_resolution_clock::now();

    duration<float> frameTime = _time - _time_old;
    _time_old = _time;

    fixed_delta = frameTime.count();
}