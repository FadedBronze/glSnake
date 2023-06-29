#pragma once
#include "Window.h"
#include "Camera.h"

class FlyCamera
{
public: 
	float cameraSpeed;
private:
	Camera& camera;
	Window& window;

public:
	FlyCamera(Window& window, Camera& camera, float cameraSpeed);
	void fly();
};

