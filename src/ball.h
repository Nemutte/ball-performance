#pragma once

#include "config.h"
class Model;

class Ball
{
public:
	glm::vec2 position;
	glm::vec2 last_position;
	float radius;

	Ball(glm::vec2 pos, Model* model);
	~Ball();

	void Update(float dt);
	void Draw();

private:
	Model* model;
};

bool DetectCollision(Ball* a, Ball* b);
void SolveCollision(Ball* a, Ball* b);