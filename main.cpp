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

auto cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
auto cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
auto cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

float lastX = 0.0f, lastY = 0.0f;
constexpr float sensitivity = 0.1f;

float fov = 60.0f;
float yaw = 0.0f, pitch = 0.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

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

void error_callback(int, const char* description) { fprintf(stderr, "Error: %s\n", description); }

static void processInput(GLFWwindow* window) {
    const float cameraSpeed = 2.5f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

bool wireframe = false;
static void key_callback(GLFWwindow* window, const int key, int, const int action, int) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    if (key == GLFW_KEY_E && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_FILL : GL_LINE);
        wireframe = !wireframe;
    }
}

bool firstMouse = true;
static void mouse_callback(GLFWwindow*, const double xpos, const double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); // reversed since y-coordinates range from bottom to top
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw   += xoffset;
    pitch += yoffset;
    if(pitch > 89.0f)
        pitch =  89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

static void mouseButtonCallback (GLFWwindow* window, int, int, int) {
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        fov = fov == 45.0f ? 60.0f : 45.0f;
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
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    lastX = static_cast<float>(width) / 2.0f;
    lastY = static_cast<float>(height) / 2.0f;

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    glVertexAttribPointer(vpos_location, 3, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), nullptr);
    glEnableVertexAttribArray(vtex_location);
    glVertexAttribPointer(vtex_location, 2, GL_FLOAT, GL_FALSE, sizeof(VertexTexture), (void *)offsetof(VertexTexture, tex));

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window)) {
        processInput(window);
        const auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        std::cout << "fps:" << 1.0f / deltaTime << std::endl;
        glClearColor(0.13f, 0.9f, 0.85f, 1.f);
        glfwGetFramebufferSize(window, &width, &height);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), static_cast<float>(width)/static_cast<float>(height), 0.1f, 100.0f);

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