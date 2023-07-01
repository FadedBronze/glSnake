#pragma once
#include "Window.h"
#include "Camera.h"

class FlyCamera
{
public: 
	float cameraSpeed;
public:
	FlyCamera(Window& window, Camera& camera, float cameraSpeed);
	void fly(Window& window, Camera& camera);
};

