#include "material.h"

Material::Material(Shader* _shader) : shader(_shader) {}
void Material::use() { }
Material::~Material() { }