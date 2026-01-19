cmake_minimum_required(VERSION 3.26.0)

project(oglog_engine VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(oglog_engine
    main.cpp
    dependencies/glad/glad.c
    src/engine.cpp
    src/logic/system_manager.cpp
    src/logic/scene_manager.cpp
    src/logic/components/text_component.cpp
    src/misc/colors.cpp
    src/logic/systems/renderer_system.cpp
    src/logic/systems/transform_system.cpp
    src/logic/systems/system.cpp
    src/util/mesh.cpp
    src/util/time.cpp
    src/util/texture.cpp
    src/util/Font.cpp
    src/service/asset_manager.cpp
    src/service/shader_manager.cpp
    src/materials/material.cpp
    src/materials/material_font.cpp
    src/materials/material_default.cpp
    src/shader.cpp
)

target_include_directories(oglog_engine PRIVATE dependencies)
target_include_directories(oglog_engine PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(oglog_engine PRIVATE ${CMAKE_SOURCE_DIR}/asset)

target_link_libraries(oglog_engine PRIVATE glfw OpenGL::GL)

add_executable(asset_tool
    asset_tool.cpp
    src/service/asset_manager.cpp
    src/util/string_operations.h
    )

target_include_directories(asset_tool PRIVATE dependencies)
target_include_directories(asset_tool PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(asset_tool PRIVATE ${CMAKE_SOURCE_DIR}/asset)