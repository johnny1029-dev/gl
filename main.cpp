#include "iostream"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "linmath.h"
#include <cmath>
#include <cstdio>
#include <GLShader.h>
#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define STB_IMAGE_IMPLEMENTATION
#include <glm.hpp>
#include <matrix_transform.hpp>
#include <stb_image.h>
#include <type_ptr.hpp>


typedef struct VertexTexture
{
    vec3 pos;
    vec2 tex;
} VertexTexture;

typedef struct VertexColor
{
    vec3 pos;
    vec3 col;
} VertexColor;

// static constexpr VertexTexture vertices[4] =
// {
//     { {  0.5f,  0.5f, 0.f },  {1.f, 1.f} },
//     { {  0.5f, -0.5f, 0.f },  {1.f, 0.f} },
//     { { -0.5f, -0.5f, 0.f },  {0.f, 0.f} },
//     { { -0.5f,  0.5f, 0.f },  {0.f, 1.f} }
// };

static constexpr VertexTexture vert3DShit[] = {
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },
{ {0.5f, -0.5f, -0.5f}, {1.0f, 0.0f} },
{ {0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
{ {0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
{ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} },
{ {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f} },

{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f} },
{ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f} },
{ {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f} },
{ {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f} },
{ {-0.5f, 0.5f, 0.5f}, {0.0f, 1.0f} },
{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f} },

{ {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
{ {-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f} },
{ {-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },

{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
{ {0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
{ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
{ {0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
{ {0.5f, -0.5f, 0.5f}, {0.0f, 0.0f} },
{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },

{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },
{ {0.5f, -0.5f, -0.5f}, {1.0f, 1.0f} },
{ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f} },
{ {0.5f, -0.5f, 0.5f}, {1.0f, 0.0f} },
{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f} },
{ {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f} },

{ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} },
{ {0.5f, 0.5f, -0.5f}, {1.0f, 1.0f} },
{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
{ {0.5f, 0.5f, 0.5f}, {1.0f, 0.0f} },
{ {-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f} },
{ {-0.5f, 0.5f, -0.5f}, {0.0f, 1.0f} }
};

glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

unsigned int indices[6] = {  // note that we start from 0!
    0, 1, 3,
    1, 2, 3
};


void error_callback(int, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

bool wireframe = false;

static void key_callback(GLFWwindow* window, const int key, int, const int action, int)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_W && action == GLFW_PRESS) {
        if (wireframe)
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        wireframe = !wireframe;
    }

}

GLuint genTexture(const std::string &name) {
    int imgWidth, imgHeight, nrChannels;
    unsigned char *data = stbi_load(name.c_str(), &imgWidth, &imgHeight, &nrChannels, 0);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return texture;
}

int main() {

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 640, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    const GLuint texture = genTexture("wall.jpg");

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert3DShit), vert3DShit, GL_STATIC_DRAW);

    const Shader shader("vertex_shader.glsl", "fragment_shader.glsl");
    shader.use();

    const GLint vpos_location = glGetAttribLocation(shader.ID, "vPos");
    const GLint vtex_location = glGetAttribLocation(shader.ID, "vTex");
    const GLint model_location = glGetUniformLocation(shader.ID, "model");
    const GLint view_location = glGetUniformLocation(shader.ID, "view");
    const GLint projection_location = glGetUniformLocation(shader.ID, "projection");

    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), static_cast<void *>(nullptr));
    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), reinterpret_cast<void *>(offsetof(VertexTexture, tex)));

    auto view = glm::mat4(1.0f);
    // note that we're translating the scene in the reverse direction of where we want to move
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));


    glEnable(GL_DEPTH_TEST);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        std::cout << "fps:" << 1.0f / deltaTime << std::endl;
        // glClearColor(1.0f - 2.0f * fabs( fmod(static_cast<float>( glfwGetTime() ), 1.f) - 0.5f ), 0.f, 1.0f - ( 1.0f - 2.0f * fabs( fmod(static_cast<float>( glfwGetTime() ), 1.f) - 0.5f ) ), 1.0f);
        glClearColor(0.13f, 0.9f, 0.85f, 1.f);
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.0f);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUniformMatrix4fv(view_location, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, glm::value_ptr(projection));
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(vertex_array);
        for (auto cubePosition : cubePositions) {
            auto model = glm::mat4(1.0f);
            model = glm::translate(model, cubePosition);
            model = glm::rotate(model, static_cast<float>(-glfwGetTime()), glm::vec3(1.0f, 0.2f, 0.5f));
            glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
    exit(EXIT_SUCCESS);
}