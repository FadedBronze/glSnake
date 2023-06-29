#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"

class Camera
{
public:
	//the position of the camera
	glm::vec3 position;

	float fov;
	float yaw;
	float pitch;

	glm::vec3 front;
	glm::vec3 up;

	Shader& shader;

	Camera(glm::vec3 position, Shader& shader, float fov, float yaw, float pitch);
	~Camera();
	
	void render();
};

