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
	std::vector<float> data = CreateDataModel();
	vertex_count = data.size() / 3;

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
		//glDrawArrays(GL_LINES, 0, vertex_count);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}
}

std::vector<float> Ball::CreateDataModel()
{
	// variable for data model
	std::vector<float> data;

	std::vector<glm::vec3*> vertices;

	//glm::vec3* start = new glm::vec3(radius, 0.0, 0.0);

	// crating vertices of ball
	int count_angle = (radius + 5);
	float angle_base = M_PI / (float)count_angle;
	float angle = 0.0;

	//vertices.push_back(start);
	while (angle < M_PI)
	{
		glm::vec3* new_vertices1 = new glm::vec3(cos(angle) * radius, sin(angle) * radius, 0.0);
		vertices.push_back(new_vertices1);
		float angle_tmp = angle_base;
		if( !(sin(angle) < 1.e-5 && sin(angle) > -1.e-5))
		{
			while (angle_tmp < 2.f * M_PI - angle_base)
			{
				glm::vec3* new_vertices2 = new glm::vec3(new_vertices1->x, sin(angle_tmp + M_PI / 2.f) * new_vertices1->y, cos(angle_tmp + M_PI / 2.f) * new_vertices1->y);
				vertices.push_back(new_vertices2);
				angle_tmp += angle_base;
			}
		}
		angle += angle_base;
	}
	//printf("vertices count = %d, count_angle = %d\n", vertices.size(), count_angle);

	// create data for OpenGL from vertices to TRIANGLES
	for (int i = 1; i<count_angle*2+1; i++)
	{
		data.push_back(vertices[0]->x);
		data.push_back(vertices[0]->y);
		data.push_back(vertices[0]->z);

		data.push_back(vertices[i]->x);
		data.push_back(vertices[i]->y);
		data.push_back(vertices[i]->z);

		if (i == count_angle * 2)
		{
			data.push_back(vertices[1]->x);
			data.push_back(vertices[1]->y);
			data.push_back(vertices[1]->z);
		}
		else
		{
			data.push_back(vertices[i + 1]->x);
			data.push_back(vertices[i + 1]->y);
			data.push_back(vertices[i + 1]->z);
		}
	}
	for (int i = 0; i<count_angle - 2;i++)
	{
		int index = i * count_angle * 2 + 1;
		for (int j = index; j < index + count_angle * 2; j++)
		{
			data.push_back(vertices[j]->x);
			data.push_back(vertices[j]->y);
			data.push_back(vertices[j]->z);

			if (j == index + count_angle * 2 - 1)
			{
				data.push_back(vertices[index]->x);
				data.push_back(vertices[index]->y);
				data.push_back(vertices[index]->z);
			}
			else
			{
				data.push_back(vertices[j+1]->x);
				data.push_back(vertices[j+1]->y);
				data.push_back(vertices[j+1]->z);
			}

			data.push_back(vertices[j + count_angle * 2]->x);
			data.push_back(vertices[j + count_angle * 2]->y);
			data.push_back(vertices[j + count_angle * 2]->z);

			
			data.push_back(vertices[j + count_angle * 2]->x);
			data.push_back(vertices[j + count_angle * 2]->y);
			data.push_back(vertices[j + count_angle * 2]->z);

			if (j == index + count_angle * 2 - 1)
			{
				data.push_back(vertices[index]->x);
				data.push_back(vertices[index]->y);
				data.push_back(vertices[index]->z);

				data.push_back(vertices[index + count_angle * 2]->x);
				data.push_back(vertices[index + count_angle * 2]->y);
				data.push_back(vertices[index + count_angle * 2]->z);
			}
			else
			{
				data.push_back(vertices[j+1]->x);
				data.push_back(vertices[j+1]->y);
				data.push_back(vertices[j+1]->z);

				data.push_back(vertices[j + count_angle * 2+1]->x);
				data.push_back(vertices[j + count_angle * 2+1]->y);
				data.push_back(vertices[j + count_angle * 2+1]->z);
			}

		}
	}
	int index = vertices.size() - 1;
	for (int i = index - 1; i > index - count_angle * 2 - 1; i--)
	{
		data.push_back(vertices[index]->x);
		data.push_back(vertices[index]->y);
		data.push_back(vertices[index]->z);

		data.push_back(vertices[i]->x);
		data.push_back(vertices[i]->y);
		data.push_back(vertices[i]->z);

		if (i == index - count_angle * 2)
		{
			data.push_back(vertices[index-1]->x);
			data.push_back(vertices[index-1]->y);
			data.push_back(vertices[index-1]->z);
		}
		else
		{
			data.push_back(vertices[i - 1]->x);
			data.push_back(vertices[i - 1]->y);
			data.push_back(vertices[i - 1]->z);
		}
	}
	// create data for OpenGL from vertices to sectors
	/*
	for (int i = 0; i < vertices.size() - 1; i++)
	{
		data.push_back(vertices[i]->x);
		data.push_back(vertices[i]->y);
		data.push_back(vertices[i]->z);

		data.push_back(vertices[i + 1]->x);
		data.push_back(vertices[i + 1]->y);
		data.push_back(vertices[i + 1]->z);
	}
	*/
	// cleaning
	for (auto v : vertices) delete v;
	vertices.clear();

	return data;
}
unsigned int make_shader()
{
	std::vector<unsigned int> modules;
	const char* shaderVer = "#version 330 core\n"
		"layout(location = 0) in vec3 vertexPos;"
		"out vec3 pos;"
		"out vec3 vector_to_pos;"
		"out vec3 light_pos;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"void main()"
		"{"
		"pos = (model * vec4(vertexPos, 1.0)).xyz;"
		"vector_to_pos = vertexPos;"
		"light_pos = vec3(0.0, 0.0, 0.0);"
		"gl_Position = projection * view * model * vec4(vertexPos, 1.0);"
		"}";// gl_Position = vec4(1.0, 0.0, 0.0, 1.0);}";	gl_Position = projection * view * model * vec4(vertexPos, 1.0); }";
	const char* shaderFrag = "#version 330 core\n"
		"in vec3 pos;"
		"in vec3 vector_to_pos;"
		"in vec3 light_pos;"
		"out vec4 screenColor;"
		"void main()"
		"{"
		"vec3 normal = vector_to_pos;"
		"float max_d = 50.0;"
		"vec3 light_dir = normalize(light_pos - pos);"
		"float distance = length(light_pos - pos);"
		"float diff = max(dot(normalize(normal), light_dir), 0.0);"
		"float attenuation = clamp(1.0 - (distance / max_d), 0.0, 1.0);"
		"float light_intensity = diff * attenuation;"
		"vec4 color = vec4(1.0, 1.0, 1.0, 1.0);"
		"color.xyz *= light_intensity;"
		"screenColor = color;"
		"}";
	modules.push_back(make_module(GL_VERTEX_SHADER, shaderVer));
	modules.push_back(make_module(GL_FRAGMENT_SHADER, shaderFrag));

	unsigned int shader = glCreateProgram();
	for (unsigned int shaderModule : modules)
	{
		glAttachShader(shader, shaderModule);
	}
	glLinkProgram(shader);

	int success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
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