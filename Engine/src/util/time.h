#pragma once
#include <config_system.h>

class Time : public System {
public:
    static double delta;
    static double smooth_delta;
    static double fixed_delta;
    static double sine;
    static double cosine;
    static double time;


    void update() override;
    void fixedUpdate() override;
private:
    static double m_time_old;
    static double m_smooth_delta_factor;
};