#include <config_render.h>
#include "texture.h"
#include <iostream>

Texture::Texture(const char* filepath)
{
    data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);

    if (!data) {
        std::cerr << "Failed to load texture: " << filepath << "\n";
        return;
    }

    glGenTextures(1, &gpu_texture);

    // in practice a vao for textures
    glBindTexture(GL_TEXTURE_2D, gpu_texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Actual texture creation
    glTexImage2D(
        GL_TEXTURE_2D, // Texture type
        0, // Mipmap level (for when we want to make our own)
        GL_RGBA, // How do we store the image on the gpu (channels)
        width,
        height,
        0, // border
        GL_RGBA, // source image channels
        GL_UNSIGNED_BYTE, // source image data type
        data // loaded image data (in RAM)
    );

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    data = nullptr;
}

Texture::~Texture() {
    if (gpu_texture != 0) {
        glDeleteTextures(1, &gpu_texture);
        gpu_texture = 0;
    }
}