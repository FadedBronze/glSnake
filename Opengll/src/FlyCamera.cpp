#include "FlyCamera.h"
#include "Camera.h"
#include <iostream>

FlyCamera::FlyCamera(Window& window, Camera& camera, float cameraSpeed)
    : cameraSpeed(cameraSpeed)
{
    window.onMouseMove = [&camera, &window](MousePosition mousePos) {
        const float sensitivity = 0.05;

        camera.yaw = (mousePos.x - window.width / 2.0f) * sensitivity;
        camera.pitch = -((mousePos.y - window.height / 2.0f) * sensitivity);
    };

    window.onScroll = [&camera](ScrollOffset scrollOffset) {
        camera.fov -= (float)scrollOffset.y * 2;
        if (camera.fov < 1.0f)
            camera.fov = 1.0f;
        if (camera.fov > 90.0f)
            camera.fov = 90.0f;
    };

    window.onKeyDown = [&camera](int _){};
}

void FlyCamera::fly(Window& window, Camera& camera)
{
    auto glfwWindow = window.getGLFWWindow();

    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.position += cameraSpeed * camera.front;
    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.position -= cameraSpeed * camera.front;
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.position -= glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.position += glm::normalize(glm::cross(camera.front, camera.up)) * cameraSpeed;
}