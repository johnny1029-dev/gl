cmake_minimum_required(VERSION 3.30)
project(hopefullyRendering)

set(CMAKE_CXX_STANDARD 20)

add_executable(hopefullyRendering main.cpp)

add_library(glad STATIC libs/glad/src/gl.c)
add_library(GLShader STATIC libs/GLShader/GLShader.cpp)
target_include_directories(glad PUBLIC libs/glad/include)

add_subdirectory(libs/glfw-3.4)
include_directories(libs/glfw-3.4/include)
include_directories(libs/GLShader/include)
include_directories(libs/stb_image)
include_directories(libs/glm)
include_directories(libs/glm/gtc)
include_directories(libs/glad/include)
include_directories(libs/linmath)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad GLShader)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/deps
    ${CMAKE_CURRENT_BINARY_DIR}
)