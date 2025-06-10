#include "ball.h"
#include "model.h"

Ball::Ball(glm::vec2 pos, Model* model) : position{ pos }, last_position{ pos }, model{ model }, radius{ 5.0 }, speed{ 50.0 }
{

}
Ball::~Ball()
{
}
void Ball::Update(float dt)
{
	glm::vec2 tmp_pos = last_position;
	last_position = position;
	position += (position - tmp_pos + glm::vec2(0.0, -1.0)) * speed * dt;
}
void Ball::Draw()
{
	model->Draw(position);
}

bool DetectCollision(Ball* a, Ball* b)
{
	glm::vec2 vecAB = b->position - a->position;
	float distance = vecAB.x * vecAB.x + vecAB.y * vecAB.y;
	float sumRadius = a->radius + b->radius;
	if (distance < sumRadius * sumRadius)
		return true;
	return false;
}
void SolveCollision(Ball* a, Ball* b)
{
	glm::vec2 vecAB = b->position - a->position;
	float distance = sqrt(vecAB.x * vecAB.x + vecAB.y * vecAB.y);
	float collision = (a->radius + b->radius - distance) / 2.0;
	vecAB /= distance;
	a->position -= vecAB * collision;
	b->position += vecAB * collision;
}