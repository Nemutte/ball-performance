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
	inline static int COUNT_CYLINDERS = 0;
	glm::vec3 position;
	glm::vec3 pointA;
	glm::vec3 pointB;
	float radius;
	bool fixed;
	bool drawable;

private:
	unsigned int VAO, vertex_count;
	unsigned int VBO;

public:
	Cylinder(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r);
	Cylinder(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r);
	Cylinder(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r, bool f);
	Cylinder(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r, bool f);

	~Cylinder();

	void CreateDrawableModel();
	void Draw();

private:
	std::vector<float> CreateDataModel();
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
bool DetectCollisionCylindervsBall(Cylinder* c, Ball* b, float& distance, glm::vec3& collision_point);
bool DetectCollisionCylindervsCylinder(Cylinder* c1, Cylinder* c2);

void SolveCollisionBallvsBall(Ball* b1, Ball* b2);
void SolveCollisionCylindervsBall(Cylinder* c, Ball* b);
void SolveCollisionCylindervsBall(Cylinder* c1, Cylinder* c2);

unsigned int make_shader();
unsigned int make_module(unsigned int module_type, const char* shaderSrc);
}



