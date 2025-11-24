#pragma once
#include <config_render.h>
#include <string>
#include <shader.h>

static const glm::vec3 right   (1.0f, 0.0f, 0.0f);
static const glm::vec3 up      (0.0f, 1.0f, 0.0f);
static const glm::vec3 forward (0.0f, 0.0f, 1.0f);

static glm::ivec2 resolution (840, 480);

static glm::vec3 screen_color (0.0f, 0.0f, 0.0f);

static const std::string shader_module_path = "../src/shaders/";

extern GLFWwindow* window;

extern Shader shader;