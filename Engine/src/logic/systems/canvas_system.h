#pragma once
#include <config_system.h>
#include <config_component.h>


class Canvas_S : public System {
public:
    void update() override;
    Canvas_S();
    ~Canvas_S();
};