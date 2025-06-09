#include "model.h"

Model::Model(unsigned int shader) : shader{ shader }
{
	std::vector<float> data =
	{
		0.0, 0.0, -1.0,  0.0, -0.5,  1.0,
		0.0, 0.0, -0.5,  1.0,  0.5,  1.0,
		0.0, 0.0,  0.5,  1.0,  1.0,  0.0,
		0.0, 0.0,  1.0,  0.0,  0.5, -1.0,
		0.0, 0.0,  0.5, -1.0, -0.5, -1.0,
		0.0, 0.0, -0.5, -1.0, -1.0,  0.0
	};
	for (int i = 0; i < data.size(); i++)
	{
		data[i] *= 5.0;
	}
	vertex_count = 18;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8, (void*)0);
	glEnableVertexAttribArray(0);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Model::Draw(glm::vec2 pos)
{
	glUseProgram(shader);
	unsigned int model_position = glGetUniformLocation(shader, "position");
	glUniform2f(model_position, pos.x, pos.y);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertex_count);
}
