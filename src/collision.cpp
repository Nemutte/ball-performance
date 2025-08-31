#include "collision.h"
#include <iostream>

namespace hib
{
bool DetectCollisionBallvsBall(Ball* b1, Ball* b2, float& distance)
{
	glm::vec3 vec = b2->position - b1->position;
	distance = glm::length(vec);

	if (distance < b1->radius + b2->radius)
	{
		return true;
	}

	return false;
}

void SolveCollisionBallvsBall(Ball* b1, Ball* b2)
{
	glm::vec3 vec = b2->position - b1->position;
	float distance = 0.f;

	if (DetectCollisionBallvsBall(b1, b2, distance))
	{
		vec /= distance;
		if (distance == 0)
		{
			distance = 1.0e-8;
			vec += 1.0e-8;
		}

		float colaps_distance = b1->radius + b2->radius - distance;

		if (!b1->fixed && !b2->fixed) colaps_distance /= 2.f;

		if (!b1->fixed)
		{
			b1->position -= vec * colaps_distance;
		}
		if (!b2->fixed)
		{
			b2->position += vec * colaps_distance;
		}
	}
}

Ball::Ball(float x, float y, float z, float r) :position{ x, y, z }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_BALLS++;
}
Ball::Ball(glm::vec3 pos, float r) :position{ pos }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_BALLS++;
}
Ball::Ball(float x, float y, float z, float r, bool fixed) :position{ x, y, z }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_BALLS++;
}
Ball::Ball(glm::vec3 pos, float r, bool fixed) :position{ pos }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_BALLS++;
}

Ball::~Ball()
{
	if(drawable)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	COUNT_BALLS--;
}

void Ball::CreateDrawableModel()
{
	std::vector<float> data;

	data.push_back(-1.0f);
	data.push_back(0.0f);
	data.push_back(0.0f);

	data.push_back(1.0f);
	data.push_back(0.0f);
	data.push_back(0.0f);

	data.push_back(0.0f);
	data.push_back(1.0f);
	data.push_back(0.0f);

	data.push_back(0.0f);
	data.push_back(-1.0f);
	data.push_back(0.0f);

	vertex_count = 4;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
	glEnableVertexAttribArray(0);
	drawable = true;
}
void Ball::Draw()
{
	if (drawable)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINE, 0, vertex_count);
	}
}

unsigned int make_shader()
{
	std::vector<unsigned int> modules;
	const char* shaderSrc1 = "#version 330 core\nlayout(location = 0) in vec3 vertexPos;\nuniform mat4 model;\nuniform mat4 view;\nuniform mat4 projection;\nvoid main(){gl_Position = projection * view * model * vec4(vertexPos, 1.0);}";// gl_Position = vec4(1.0, 0.0, 0.0, 1.0);}";	gl_Position = projection * view * model * vec4(vertexPos, 1.0);}";
	const char* shaderSrc2 = "#version 330 core\nout vec4 screenColor; void main() { screenColor = vec4(0.0, 1.0, 0.0, 1.0); }";
	modules.push_back(make_module(GL_VERTEX_SHADER, shaderSrc1));
	modules.push_back(make_module(GL_FRAGMENT_SHADER, shaderSrc2));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetShaderiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetProgramInfoLog(shader, 1024, NULL, errorLog);
		std::cout << "Shader linking error: " << errorLog << std::endl;
	}
	for (unsigned int shaderModule : modules)
	{
		glDeleteShader(shaderModule);
	}

	return shader;
}
unsigned int make_module(unsigned int module_type, const char* shaderSrc)
{
	unsigned int shaderModule = glCreateShader(module_type);
	glShaderSource(shaderModule, 1, &shaderSrc, NULL);
	glCompileShader(shaderModule);

	int success;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char errorLog[1024];
		glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
		std::cout << "Shader Module compilation error: " << errorLog << std::endl;
	}

	return shaderModule;
}

}