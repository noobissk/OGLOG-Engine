

class System {
private:
    bool is_enabled;

public:

    bool isEnabled() const { return is_enabled; }

    /// @brief run every frame
    void update();

    /// @brief runs when class is enabled, before the first update
    void start();

    /// @brief runs during project startup
    void awake();

    /// @brief runs every time the system is enabled -- on Start the system is enabled by default (unless set up otherwise)
    void onEnable();
    
    /// @brief runs every time the system is disabled -- on Start the system is enabled by default (unless set up otherwise)
    void onDisable();

    /// @brief enables this system to run update every frame
    void enable();

    /// @brief disables this systems update to run every frame
    void disable();
};