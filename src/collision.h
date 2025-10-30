#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

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

class Capsule
{
public:
	inline static int COUNT_CapsuleS = 0;
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
	Capsule(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r);
	Capsule(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r);
	Capsule(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r, bool f);
	Capsule(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r, bool f);

	~Capsule();

	void CreateDrawableModel();
	void Draw();

private:
	std::vector<float> CreateDataModel();
};

class Ray3d
{
public:
	glm::vec3 position;
	glm::vec3 ray;
	bool drawable;

private:
	unsigned int VAO, vertex_count;
	unsigned int VBO;

public:
	Ray3d(glm::vec3 pos, glm::vec3 ray);
	Ray3d(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2);
	Ray3d(float px, float py, float pz, float rx, float ry, float rz);

	~Ray3d();

	void CreateDrawableModel();
	void Draw();
};

class Polygon3d
{
public:
	std::vector<glm::vec3*> points;
	glm::vec3* normal;

	Polygon3d(std::vector<glm::vec3*> p, glm::vec3* normal);
	~Polygon3d();
};

class PolygonFigure3d
{
public:
	std::vector<Polygon3d*> poligons;
	std::vector<glm::vec3*> vertices;
	bool drawable;

private:
	unsigned int VAO, vertex_count;
	unsigned int VBO;

public:
	PolygonFigure3d(const char* filename);
	~PolygonFigure3d();

	void LoadModelData(const char* filename,
		std::vector<double>& vertices,
		std::vector<double>& normals,
		std::vector<int>& faces_v,
		std::vector<int>& faces_vn,
		int& v_count,
		int& vn_count,
		int& f_count);
	void CreateDrawableModel();
	void Draw();
};

bool DetectCollisionBallvsBall(Ball* b1, Ball* b2, float& distance);
bool DetectCollisionCapsulevsBall(Capsule* c, Ball* b, float& distance, glm::vec3& collision_point);
bool DetectCollisionCapsulevsCapsule(Capsule* c1, Capsule* c2, glm::vec3& collision_pointA, glm::vec3& collision_pointB, float& distance);
bool DetectCollisionRayvsBall(Ray3d* ray, Ball* ball, float& distance_from_source_of_ray);
bool DetectCollisionRayvsCapsule(Ray3d* ray, Capsule* cap, glm::vec3& collision_pointA, glm::vec3& collision_pointB, float& distance_from_source_of_ray);

void SolveCollisionBallvsBall(Ball* b1, Ball* b2);
void SolveCollisionCapsulevsBall(Capsule* c, Ball* b);
void SolveCollisionCapsulevsCaplsule(Capsule* c1, Capsule* c2);

unsigned int make_shader();
unsigned int make_module(unsigned int module_type, const char* shaderSrc);
std::vector<std::string> split(std::string line, std::string delimiter);
}



