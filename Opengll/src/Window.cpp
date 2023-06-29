#include "GL/glew.h"
#include "Window.h"
#include "GLFW/glfw3.h"
#include "Renderer.h"
#include <iostream>

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

Window* get_context(GLFWwindow* w) {
    // Cast the user pointer of the window to callback_context type and return it
    return static_cast<Window*>(glfwGetWindowUserPointer(w));
}

void ScrollCallback(GLFWwindow* glfwWindow, double xoffset, double yoffset)
{
    get_context(glfwWindow)->onScroll(ScrollOffset{
        xoffset,
        yoffset,
    });
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* glfwWindow, double xpos, double ypos)
{
    get_context(glfwWindow)->onMouseMove(MousePosition{
        xpos,
        ypos
    });
}

void KeypressCallback(GLFWwindow* glfwWindow, int key, int scancode, int action, int mods)
{
    auto window = get_context(glfwWindow);

    window->onKeyDown(key);
}

Window::Window(int width, int height)
    : width(width), height(height)
{
    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "GLFW INIT FAILED\n" << std::endl;
        glfwTerminate();
        ASSERT(false);
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    glfwWindow = glfwCreateWindow(width, height, "Hello World", NULL, NULL);

    if (!glfwWindow)
    {
        std::cout << "GLFW INIT FAILED\n" << std::endl;
        glfwTerminate();
        ASSERT(false);
        return;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(glfwWindow);

    glfwSwapInterval(1);

    glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetScrollCallback(glfwWindow, ScrollCallback);

    glfwSetFramebufferSizeCallback(glfwWindow, FramebufferSizeCallback);

    glfwSetCursorPosCallback(glfwWindow, MouseCallback);

    glfwSetKeyCallback(glfwWindow, KeypressCallback);

    glfwSetWindowUserPointer(glfwWindow, this);

    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW INIT FAILED\n" << std::endl;
        ASSERT(false);
        return;
    }
}

Window::~Window()
{
    glfwTerminate();
}

void Window::Update(const std::function<void(float)>& update)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    while (!glfwWindowShouldClose(glfwWindow)) {
        update(deltaTime);
        glfwSwapBuffers(glfwWindow);
        glfwPollEvents();
    }
}