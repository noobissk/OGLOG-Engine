

#pragma once


class Time {
public:
    static float delta;
    static float smooth_delta;
    static float fixed_delta;
    static float sine;
    static float cosine;
    static float time;


    void update();
    void updateFixed();
private:
    static float m_time_old;
    static float m_smooth_delta_factor;
};