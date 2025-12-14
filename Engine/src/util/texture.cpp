#include <config_render.h>
#include "texture.h"
#include <iostream>

Texture::Texture(std::filesystem::path filepath)
{
    file_name = filepath.string();
    width = 0; height = 0; channels = 0; data = nullptr; gpu_texture = 0;
}

Texture::~Texture() {
    if (gpu_texture != 0) {
        glDeleteTextures(1, &gpu_texture);
        gpu_texture = 0;
    }
}

void Texture::uploadIfNeeded(int _desired_channels)
{
    if (glIsTexture(gpu_texture)) {
        // already uploaded
        return;
    }
    int w, h, ch;
    unsigned char* img = stbi_load(file_name.c_str(), &w, &h, &ch, _desired_channels);
    if (!img) {
        std::cerr << "[ERROR] uploadIfNeeded: failed to load image -> " << file_name << std::endl;
        return;
    }

    width = w; height = h; channels = ch;

    glGenTextures(1, &gpu_texture);
    if (gpu_texture == 0) {
        std::cerr << "[ERROR] uploadIfNeeded: glGenTextures returned 0" << std::endl;
        stbi_image_free(img);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, gpu_texture);
    if (_desired_channels == 1) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, img);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
    }
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        std::cerr << "[ERROR] uploadIfNeeded: glTexImage2D GL error: 0x" << std::hex << err << std::dec << std::endl;
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);
}