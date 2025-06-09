#pragma once
#include "config.h"

class Model
{
public:
	Model(unsigned int shader);
	~Model();

	void Draw(glm::vec2 pos);
private:
	unsigned int VAO, VBO, vertex_count;
	unsigned int shader;
};