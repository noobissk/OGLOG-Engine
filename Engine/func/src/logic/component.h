

class Component {
public:
    int game_object;
    bool enabled;

    void Awake();
    void Start();
    void Update();
    void OnDelete();
    void OnEnable();
    void OnDisable();
};

