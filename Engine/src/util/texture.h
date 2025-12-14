#pragma once
#include <string>
#include <filesystem>

struct Texture {
    std::string file_name;
    int height;
    int width;
    int channels;
    unsigned char* data;
    unsigned int gpu_texture;
    Texture(std::filesystem::path filepath);
    ~Texture();
    void uploadIfNeeded(int _desired_channels);
};