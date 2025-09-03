#pragma once
#include "config.h"

class Camera
{
public:
	bool w, a, s, d, space_bar, shift;
	float speed = 15.f;
	glm::vec3 position;
	Camera(glm::vec3 pos);
	~Camera();
	void Move(float dt);
	glm::vec3 GetCameraTarget();
	void Rotate(float alfa, float beta);

	glm::vec3 camera_direction;
	glm::vec3 horisontal_direction;
private:
	glm::vec3 camera_target;
};