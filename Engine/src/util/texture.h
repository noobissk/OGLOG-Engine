#pragma once
#include <string>

struct Texture {
public:
    std::string file_name;
    int height;
    int width;
    int channels;
    unsigned char* data;
    unsigned int gpu_texture;
    Texture(const char* filepath);
    ~Texture();
};