#include "camera.h"

Camera::Camera(glm::vec3 pos) :position{ pos }
{
	w = false;
	a = false;
	s = false;
	d = false;
	space_bar = false;
	shift = false;

	camera_direction = { 0.0, 0.0, -1.0 };
	horisontal_direction = { 0.0, 0.0, -1.0 };
	camera_target = position + camera_direction;
}
Camera::~Camera()
{
}
glm::vec3 Camera::GetCameraTarget()
{
	camera_target = position + camera_direction;
	return camera_target;
}
void Camera::Rotate(float alfa, float beta)
{
	camera_direction.x = cos(beta) * sin(alfa);
	camera_direction.y = -sin(beta);
	camera_direction.z = -1 * (cos(beta) * cos(alfa));
	camera_direction = glm::normalize(camera_direction);

	horisontal_direction.x = sin(alfa);
	horisontal_direction.y = 0;
	horisontal_direction.z = -1 * cos(alfa);
	horisontal_direction = glm::normalize(horisontal_direction);
}
void Camera::Move(float dt)
{
	glm::vec3 move_vector = { 0.f, 0.f, 0.f };
	if (w)
		move_vector += glm::vec3(horisontal_direction.x, 0.f, horisontal_direction.z);
	if (s)
		move_vector += glm::vec3(-horisontal_direction.x, 0.f, -horisontal_direction.z);
	if (a)
		move_vector += glm::vec3(horisontal_direction.z, 0.f, -horisontal_direction.x);
	if (d)
		move_vector += glm::vec3(-horisontal_direction.z, 0.f, horisontal_direction.x);
	if (space_bar)
		move_vector += glm::vec3(0.0f, 1.f, 0.0f);
	if (shift)
		move_vector += glm::vec3(0.0f, -1.f, 0.0f);

	if (move_vector.length() != 0.f)
	{
		move_vector /= (float)move_vector.length();
		position += move_vector * speed * dt;
	}
}