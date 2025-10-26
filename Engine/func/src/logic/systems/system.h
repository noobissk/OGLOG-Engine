#pragma once
#include <config_component.h>

class System {
private:
    bool is_enabled;

public:
    int id;

    virtual bool isEnabled() const { return is_enabled; }

    /// @brief run every frame
    virtual void update();

    /// @brief runs at fixed intervals set in engine_values.h
    virtual void fixedUpdate();

    /// @brief runs when class is enabled, before the first update
    virtual void start();

    /// @brief runs during project startup
    virtual void awake();

    /// @brief runs every time the system is enabled -- on Start the system is enabled by default (unless set up otherwise)
    virtual void onEnable();

    /// @brief runs every time the system is disabled -- on Start the system is enabled by default (unless set up otherwise)
    virtual void onDisable();

    /// @brief enables this system to run update every frame
    virtual void enable();

    /// @brief disables this systems update to run every frame
    virtual void disable();

    System();
    virtual ~System();
};