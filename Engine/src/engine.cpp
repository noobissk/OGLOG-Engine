#include <engine.h>
#include <config_system.h>
#include <logic/system_manager.h>
#include <logic/scene_manager.h>
#include <misc/colors.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <service/asset_manager.h>


GLFWwindow* Engine::window = nullptr;


unsigned int current_shader = 0;




int Engine::startUp()
{
    AssetManager::start();
    current_shader = createShader(shader_module_path + "vertex.glsl", shader_module_path + "fragment.glsl");
    

    SceneManager::createScene("default");
    SystemManager::initialize();
    SystemManager::awake();

    screen_color = Colors::program_default;
    glClearColor(screen_color.r, screen_color.g, screen_color.b, 1.0f);
    glUseProgram(current_shader);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        glClear(GL_COLOR_BUFFER_BIT);

        SystemManager::update();

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    Engine::quit();

    return 0;
}

void Engine::quit() { }



unsigned int Engine::createModule(const std::string& filepath, unsigned int module_type) {
    std::ifstream file;
    std::stringstream buffered_lines;
    std::string line;

    std::cout << "[LOG] Reading shader -> " << filepath << '\n';

    file.open(filepath);
    while (std::getline(file, line)) {
        buffered_lines << line << '\n';
    }
    std::string shader_source = buffered_lines.str();
    const char* shader_src = shader_source.c_str();

    buffered_lines.str("");
    file.close();

    unsigned int shader_module = glCreateShader(module_type);
    glShaderSource(shader_module, 1, &shader_src, NULL);
    glCompileShader(shader_module);


    int success;
    glGetShaderiv(shader_module, GL_COMPILE_STATUS, &success);
    if (!success) {
        char error_log[1024];
        glGetShaderInfoLog(shader_module, 1024, NULL, error_log);
        std::cout << "[ERROR] Shader module compilation failed -> " << error_log << std::endl;
    }

    return shader_module;
}


unsigned int Engine::createShader(const std::string& vertex_filepath, const std::string& fragment_filepath) {
    std::vector<unsigned int> modules;

    std::cout << "[LOG] Creating shader modules." << std::endl;
    
    modules.push_back(createModule(vertex_filepath, GL_VERTEX_SHADER));
    modules.push_back(createModule(fragment_filepath, GL_FRAGMENT_SHADER));

    std::cout << "[LOG] Shader module creation successful." << std::endl;

    unsigned int shader = glCreateProgram();
    for (unsigned int shader_module : modules)
    {
        glAttachShader(shader, shader_module);
    }
    
    glLinkProgram(shader);
    
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char error_log[1024];
        glGetProgramInfoLog(shader, 1024, NULL, error_log);
        std::cout << "[ERROR] Shader linking failed -> " << error_log << std::endl;
    }
    
    for (unsigned int module : modules)
    {
        glDeleteShader(module);
    }
    
    std::cout << "[LOG] Shader linking successful." << std::endl;
    return shader;
}