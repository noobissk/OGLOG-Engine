cmake_minimum_required(VERSION 3.26.0)

project(oglog_engine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(oglog_engine
    func/main.cpp
    dependencies/glad/glad.c
    func/src/engine.cpp
    func/src/logic/system_manager.cpp
    func/src/logic/scene_manager.cpp
    func/config.cpp
    func/src/misc/colors.cpp
    func/src/logic/systems/render_system.cpp
    func/src/logic/systems/transform_system.cpp
    func/src/logic/systems/system.cpp
    func/src/util/mesh.cpp
    func/src/util/time.cpp

    )

target_include_directories(oglog_engine PRIVATE dependencies)
target_include_directories(oglog_engine PRIVATE ${CMAKE_SOURCE_DIR}/func/src)
target_include_directories(oglog_engine PRIVATE ${CMAKE_SOURCE_DIR}/func/incl)
target_include_directories(oglog_engine PRIVATE ${CMAKE_SOURCE_DIR}/asset)

target_link_libraries(oglog_engine PRIVATE glfw OpenGL::GL)
