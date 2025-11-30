#include "ball.h"
#include "model.h"

Circle::Circle(glm::vec2 pos, Model* model) : position{ pos }, last_position{ pos }, model{ model }, radius{ 5.0 }, speed{ 100.0 }
{

}
Circle::~Circle()
{
}

void Circle::Update(float dt)
{
	glm::vec2 tmp_pos = last_position;
	last_position = position;
	position += (position - tmp_pos + glm::vec2(0.0, -1.0)) * speed * dt;
}
void Circle::Draw()
{
	model->Draw(position);
}

bool DetectCollision(Circle* a, Circle* b)
{
	glm::vec2 vecAB = b->position - a->position;
	float distance = vecAB.x * vecAB.x + vecAB.y * vecAB.y;
	float sumRadius = a->radius + b->radius;
	if (distance < sumRadius * sumRadius)
		return true;
	return false;
}
void SolveCollision(Circle* a, Circle* b)
{
	glm::vec2 vecAB = b->position - a->position;
	float distance = sqrt(vecAB.x * vecAB.x + vecAB.y * vecAB.y);
	float collision = (a->radius + b->radius - distance) / 2.0;
	vecAB /= distance;
	a->position -= vecAB * collision;
	b->position += vecAB * collision;
}

Ball::Ball(glm::vec3 pos, float r) : position{ pos }, radius{ r }, speed{ 4.0 }
{
	last_position = position;
	collision_body = new hib::Ball(pos, r);
}
Ball::~Ball()
{
	delete collision_body;
}
void Ball::Draw()
{
	collision_body->Draw();
}
void Ball::Update(float dt)
{
	glm::vec3 tmp_pos = last_position;
	last_position = position;
	glm::vec3 move_vector = glm::normalize(position - tmp_pos + glm::vec3(0.0, -0.05, 0.0));
	position += move_vector * speed * dt;
	collision_body->position = position;
}

Figure3d::Figure3d(const char* filename, float x, float y, float z, bool fixed)
{
	position = glm::vec3(x, y, z);
	last_position = position;
	speed = 4.f;
	collision_body = new hib::PolygonFigure3d(filename, x, y, z, fixed);
}
Figure3d::~Figure3d()
{
	delete collision_body;
}
void Figure3d::Update(float dt)
{
	glm::vec3 tmp_pos = last_position;
	last_position = position;
	glm::vec3 move_vector = glm::normalize(position - tmp_pos + glm::vec3(0.0, -0.05, 0.0));
	position += move_vector * speed * dt;
	collision_body->position = position;
}
void Figure3d::Draw()
{
	collision_body->Draw();
}