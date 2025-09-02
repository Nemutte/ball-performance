#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>


#define M_PI  3.14159265

namespace hib
{
class Ball
{
public:
	inline static int COUNT_BALLS = 0;
	glm::vec3 position;
	float radius;
	bool fixed;
	bool drawable;

private:
	unsigned int VAO, vertex_count;
	unsigned int VBO;

public:
	Ball(float x, float y, float z, float r);
	Ball(glm::vec3 pos, float r);
	Ball(float x, float y, float z, float r, bool fixed);
	Ball(glm::vec3 pos, float r, bool fixed);
	
	~Ball();

	void CreateDrawableModel();
	void Draw();

private:
	std::vector<float> CreateDataModel();
};

class Cylinder
{
public:
	glm::vec3 pointA;
	glm::vec3 pointB;
	float radius;
};

class Ray3d
{
public:
	glm::vec3 point;
	glm::vec3 ray;
};

class Polygon3d
{
public:

};

class PolygonFigure3d
{
public:

};

bool DetectCollisionBallvsBall(Ball* b1, Ball* b2, float& distance);

void SolveCollisionBallvsBall(Ball* b1, Ball* b2);

unsigned int make_shader();
unsigned int make_module(unsigned int module_type, const char* shaderSrc);
}



