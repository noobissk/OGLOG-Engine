#include "config.h"
#include "scripts/shader_modules.h"
#include "scripts/triangle_mesh.h"
#include "scripts/linear_algebra.h"
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
    
    vec3 quad_position = {0.0f, 0.0f, 0.0f};
    mat4 model = create_matrix_transform(quad_position);
    unsigned int model_location = glGetUniformLocation(shader, "model");
    unsigned int view_location = glGetUniformLocation(shader, "view");
    
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        
        mat4 model = create_model_transform( {0.0f, 0.0f, 0.0f}, 0);
        glUniformMatrix4fv(model_location, 1, GL_FALSE, model.entries);


        vec3 camera_pos = {cosf(glfwGetTime()) * 0.7f, sinf(glfwGetTime()) * 0.7f, 1.0f};
        vec3 camera_target = {0.0f, 0.0f, 0.0f};
        mat4 view = create_look_at(camera_pos, camera_target);
        glUniformMatrix4fv(view_location, 1, GL_FALSE, view.entries);

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