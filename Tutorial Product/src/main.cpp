#include "config.h"
#include "scripts/shader_modules.h"
#include "scripts/triangle_mesh.h"
#include "material.h"

GLFWwindow* window;
static const std::string shaderPath = "../src/shaders/";

static int width = 640, height = 400;

static glm::vec3 up = {0, 1, 0};
static glm::vec3 right = {1, 0, 0};
static glm::vec3 forward = {0, 0, 1};

int main ()
{
    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(width, height, "OGLOG Engine project", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couln't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.15f, 0.25f, 0.65f, 1.0f);
    
    unsigned int shader = make_shader(
        shaderPath + "vertex.txt",
        shaderPath + "fragment.txt"
    );

    TriangleMesh* triangle = new TriangleMesh();
    Material* material = new Material("../src/textures/image.png");
    Material* mask = new Material("../src/textures/mask.png");

    
    // set texture units
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"), 0);
    glUniform1i(glGetUniformLocation(shader, "mask"), 1);

    unsigned int model_location = glGetUniformLocation(shader, "model");
    unsigned int view_location = glGetUniformLocation(shader, "view");
    unsigned int projection_location = glGetUniformLocation(shader, "projection");

    glm::vec3 quad_position = { 0.0f, 0.0f, 0.0f };

    glm::mat4 projection = glm::perspective(90.0f, ((float)width)/((float)height), 0.1f, 10.0f);
    glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, quad_position);
        model = glm::rotate(model, (float)0.0f, { 0.0f, 0.0f, 1.0f }); // in math, translate & rotate should be swapped, idk why glm has them in opposite order
        glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));


        glm::vec3 camera_pos = {cosf(glfwGetTime()) * 0.7f, sinf(glfwGetTime()) * 0.7f, 1.0f};
        glm::vec3 camera_target = {0.0f, 0.0f, 0.0f};
        glm::mat4 view = glm::lookAt(camera_pos, camera_target, up);
        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));

        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);

        material->use(0);
        mask->use(1);

        triangle->draw();

        glfwSwapBuffers(window);
    }

    glDeleteProgram(shader);
    glfwTerminate();
    delete material;
    delete mask;
    delete triangle;

    return 0;
}