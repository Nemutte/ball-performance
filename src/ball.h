#pragma once

#include "config.h"
class Model;

class Circle
{
public:
	glm::vec2 position;
	glm::vec2 last_position;
	float radius;
	float speed;

	Circle(glm::vec2 pos, Model* model);
	~Circle();

	void Update(float dt);
	void Draw();

private:
	Model* model;
};

bool DetectCollision(Circle* a, Circle* b);
void SolveCollision(Circle* a, Circle* b);