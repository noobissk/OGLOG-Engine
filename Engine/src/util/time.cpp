#include <util/time.h>
#include <chrono>
#include <cmath>


double Time::delta = 0.0f;
double Time::fixed_delta = 0.0f;
double Time::smooth_delta = 0.0f;
double Time::sine = 0.0f;
double Time::cosine = 0.0f;
double Time::time = 0.0f;
double Time::m_time_old = 0.0f;
double Time::m_smooth_delta_factor = 0.1f;


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
    sine = std::sin(time);
    cosine = std::cos(time);
}

void Time::fixedUpdate()
{
    static auto _time_old = high_resolution_clock::now();
    auto _time = high_resolution_clock::now();

    duration<float> frameTime = _time - _time_old;
    _time_old = _time;

    fixed_delta = frameTime.count();
}