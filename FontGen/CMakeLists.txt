cmake_minimum_required(VERSION 3.26.0)

project(oglog_fontgen VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

find_package(OpenCV REQUIRED)

add_executable(oglog_fontgen 
    src/main.cpp
    src/FileParser.cpp
    src/GlyphData.cpp
)

message("OpenCV include dirs: ${OpenCV_INCLUDE_DIRS}")

target_include_directories(oglog_fontgen
    PRIVATE
        dependencies
        ${CMAKE_SOURCE_DIR}/src
        ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(oglog_fontgen PRIVATE ${OpenCV_LIBS})