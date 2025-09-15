#include "material.h"
#include "stb_image.h"

Material::Material(const char* filename)
{
    // STBI_rgb_alpha standardises images to all have rgba channels at the cost of higher mem aloc
    // all ints get set during stbi_load. This way we can set multiple values using a single method.
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, STBI_rgb_alpha);

    // make the texture ↑ after it is loaded ↓
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // send data to GPU
    glTexImage2D(GL_TEXTURE_2D, 
        0, // mipmap level !!!
        GL_RGBA, // texture channels (what we loaded earlier)
        width,
        height,
        0, // border
        GL_RGBA, // image channels
        GL_UNSIGNED_BYTE,
        data // pointer to mem loc
    );

    // free data
    stbi_image_free(data);


    // configure sampler
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Material::~Material()
{
    glDeleteTextures(1, &texture); // number of deleting textures | texture mem loc
}

void Material::use(int unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
}