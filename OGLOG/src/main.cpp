#include "config.h"
#include "scripts/shader_modules.h"
#include "scripts/triangle_mesh.h"
#include "material.h"

GLFWwindow* window;
static const std::string shaderPath = "../../src/shaders/";

int main ()
{
    if (!glfwInit())
    {
        return -1;
    }

    window = glfwCreateWindow(640, 400, "OGLOG Engine project", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couln't load OpenGL" << std::endl;
        glfwTerminate();
        return -1;
    }

    glClearColor(0.15f, 0.25f, 0.65f, 1.0f);
    
    unsigned int shader = make_shader(
        shaderPath +  "vertex.txt",
        shaderPath + "fragment.txt"
    );

    TriangleMesh* triangle = new TriangleMesh();
    Material* material = new Material("../../src/textures/image.png");
    Material* mask = new Material("../../src/textures/mask.png");


    // set texture units
    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "material"), 0);
    glUniform1i(glGetUniformLocation(shader, "mask"), 1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

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