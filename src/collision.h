#pragma once
#include <glm/glm.hpp>

class Ball
{
public:
	glm::vec3 position;
	float radius;
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