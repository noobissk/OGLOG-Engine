#pragma once
#include <string>
#include <filesystem>

struct Texture {
public:
    std::string file_name;
    int height;
    int width;
    int channels;
    unsigned char* data;
    unsigned int gpu_texture;
    Texture(std::filesystem::path filepath);
    ~Texture();
    // Ensure the image is uploaded to GPU (create texture) in the current GL context
    void uploadIfNeeded();
};