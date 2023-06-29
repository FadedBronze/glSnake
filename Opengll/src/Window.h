#pragma once
#include <functional>
#include <GLFW/glfw3.h>

struct ScrollOffset {
	double x;
	double y;
};

struct MousePosition {
	double x;
	double y;
};

class Window
{
public:
	Window(int width, int height);
	~Window();
	int width;
	int height;
	void Update(const std::function<void(float)>& update);
	std::function<void(MousePosition)> onMouseMove;
	std::function<void(ScrollOffset)> onScroll;
	std::function<void(int)> onKeyDown;
	inline GLFWwindow* getGLFWWindow() { return glfwWindow; };
private:
	GLFWwindow* glfwWindow;
};

