struct Canvas_C {
    float reference_width;
    float reference_height;
    Canvas_C() : reference_width(1920.0f), reference_height(1080.0f) {}
    Canvas_C(float ref_w, float ref_h) : reference_width(ref_w), reference_height(ref_h) {}
};