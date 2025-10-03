#include "collision.h"
#include <iostream>

namespace hib
{
float min(float a, float b)
{
	if (a <= b)return a;
	return b;
}
float max(float a, float b)
{
	if (a >= b)return a;
	return b;
}
glm::vec3 ClosestPointOnLineSegment(glm::vec3 A, glm::vec3 B, glm::vec3 Point)
{
	glm::vec3 AB = B - A;
	float t = glm::dot(Point - A, AB) / glm::dot(AB, AB);
	return A + AB * min(max(t, 0.0), 1.0); // saturate(t) can be written as: min((max(t, 0), 1)
}

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
bool DetectCollisionCapsulevsBall(Capsule* c, Ball* b, float& distance, glm::vec3& collision_point)
{
	glm::vec3 vec_Capsule_dir = c->pointB - c->pointA;
	glm::vec3 vec_to_ball = b->position - (c->position + c->pointA);
	float length_vec_Capsule_dir = glm::length(vec_Capsule_dir);
	float distance_on_vec_dir = glm::dot(glm::normalize(vec_Capsule_dir), vec_to_ball);
	if (distance_on_vec_dir >= length_vec_Capsule_dir)
	{
		collision_point = c->position + c->pointB;
		glm::vec3 vec = b->position - collision_point;
		distance = glm::length(vec);
		if (distance < c->radius + b->radius)
		{
			return true;
		}

		return false;
	}
	else if (distance_on_vec_dir <= 0.0)
	{
		collision_point = c->position + c->pointA;
		glm::vec3 vec = b->position - collision_point;
		distance = glm::length(vec);

		if (distance < c->radius + b->radius)
		{
			return true;
		}

		return false;
	}
	else
	{
		collision_point = c->position + c->pointA + glm::normalize(vec_Capsule_dir) * distance_on_vec_dir;
		glm::vec3 vec = b->position - collision_point;
		distance = glm::length(vec);

		if (distance < c->radius + b->radius)
		{
			return true;
		}

		return false;
	}
	
	return false;
}
bool DetectCollisionCapsulevsCapsule(Capsule* c1, Capsule* c2, glm::vec3& collision_pointA, glm::vec3& collision_pointB, float& colaps_distance)
{
	glm::vec3 A = c1->position + c1->pointA;
	glm::vec3 B = c1->position + c1->pointB;
	glm::vec3 C = c2->position + c2->pointA;
	glm::vec3 D = c2->position + c2->pointB;

	glm::vec3 u = B - A;
	glm::vec3 v = D - C;
	glm::vec3 w = A - C;

	float a = glm::dot(u, u);
	float b = glm::dot(u, v);
	float c = glm::dot(v, v);
	float d = glm::dot(u, w);
	float e = glm::dot(v, w);
	float Dn = a * c - b * b;

	float s, t;

	if (Dn < 1e-9f)
	{
		s = 0.0f;
		t = (b > c ? d / b : e / c);
	}
	else
	{
		s = (b * e - c * d) / Dn;
		t = (a * e - b * d) / Dn;
	}

	s = glm::clamp(s, 0.0f, 1.0f);
	t = glm::clamp(t, 0.0f, 1.0f);
	
	collision_pointA = A + u * s;
	collision_pointB = C + v * t;
	float distance = glm::length(collision_pointA - collision_pointB);
	colaps_distance = c1->radius + c2->radius - distance;

	return distance <= (c1->radius+c2->radius);
}
bool DetectCollisionRayvsBall(Ray3d* ray, Ball* ball, float& distance_from_source_of_ray)
{
	glm::vec3 vec_Capsule_dir = ray->ray;
	glm::vec3 vec_to_ball = ball->position - ray->position;
	float distance_on_vec_dir = glm::dot(vec_Capsule_dir, vec_to_ball);
	glm::vec3 collision_point;

	distance_from_source_of_ray = 0;

	if (distance_on_vec_dir <= 0.0)
	{
		collision_point = ray->position;
		glm::vec3 vec = ball->position - collision_point;
		float distance = glm::length(vec);

		if (distance < ball->radius)
		{
			distance_from_source_of_ray = glm::dot(vec_Capsule_dir, collision_point - ray->position);
			return true;
		}

		return false;
	}
	else
	{
		collision_point = ray->position + vec_Capsule_dir * distance_on_vec_dir;
		glm::vec3 vec = ball->position - collision_point;
		float distance = glm::length(vec);

		if (distance < ball->radius)
		{
			distance_from_source_of_ray = glm::dot(vec_Capsule_dir, collision_point - ray->position);
			return true;
		}

		return false;
	}
	return false;
}
bool DetectCollisionRayvsCapsule(Ray3d* ray, Capsule* cap, glm::vec3& collision_pointA, glm::vec3& collision_pointB, float& distance_from_source_of_ray)
{
	glm::vec3 A = cap->position + cap->pointA;
	glm::vec3 B = cap->position + cap->pointB;
	glm::vec3 C = ray->position;

	glm::vec3 u = B - A;
	glm::vec3 v = ray->ray;
	glm::vec3 w = A - C;

	float a = glm::dot(u, u);
	float b = glm::dot(u, v);
	float c = glm::dot(v, v);
	float d = glm::dot(u, w);
	float e = glm::dot(v, w);
	float Dn = a * c - b * b;

	float s, t;

	if (Dn < 1e-9f)
	{
		s = 0.0f;
		t = (b > c ? d / b : e / c);
	}
	else
	{
		s = (b * e - c * d) / Dn;
		t = (a * e - b * d) / Dn;
	}

	s = glm::clamp(s, 0.0f, 1.0f);

	collision_pointA = A + u * s;
	collision_pointB = C + v * t;
	float distance = glm::length(collision_pointA - collision_pointB);

	distance_from_source_of_ray = glm::dot(collision_pointA - ray->position, ray->ray);
	printf("collision_pointA x = %f, y = %f, z = %f\n", collision_pointA.x, collision_pointA.y, collision_pointA.z);
	printf("collision_pointB x = %f, y = %f, z = %f\n", collision_pointB.x, collision_pointB.y, collision_pointB.z);
	printf("s = %f, t = %f, distance = %f\n", s, t, distance);
	return distance <= cap->radius;
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
void SolveCollisionCapsulevsBall(Capsule* c, Ball* b)
{
	glm::vec3 collision_point;
	float distance = 0.f;

	if (DetectCollisionCapsulevsBall(c, b, distance, collision_point))
	{
		glm::vec3 vec = b->position - collision_point;
		vec /= distance;
		if (distance == 0.0)
		{
			distance = 1.0e-8;
			vec += 1.0e-8;
		}

		float colaps_distance = b->radius + c->radius - distance;

		if (!c->fixed && !b->fixed) colaps_distance /= 2.f;

		if (!c->fixed)
		{
			c->position -= vec * colaps_distance;
		}
		if (!b->fixed)
		{
			b->position += vec * colaps_distance;
		}
	}
}
void SolveCollisionCapsulevsCaplsule(Capsule* c1, Capsule* c2)
{
	float colaps_distance;
	glm::vec3 cA;
	glm::vec3 cB;

	if (DetectCollisionCapsulevsCapsule(c1, c2, cA, cB, colaps_distance))
	{
		glm::vec3 vec = glm::normalize(cB - cA);
		if (colaps_distance == 0.0)
		{
			colaps_distance = 1.0e-8;
			vec += 1.0e-8;
		}
		if (!c1->fixed && !c2->fixed) colaps_distance /= 2.f;

		if (!c1->fixed)
		{
			c1->position -= vec * colaps_distance;
		}
		if (!c2->fixed)
		{
			c2->position += vec * colaps_distance;
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

	// crating vertices of ball
	int count_angle = (radius + 5);
	float angle_base = M_PI / (float)count_angle;

	for (int i = 0; i <= count_angle; i++)
	{
		glm::vec3* new_vertices1 = new glm::vec3(cos(angle_base*i) * radius, sin(angle_base * i) * radius, 0.0);
		vertices.push_back(new_vertices1);
		if( i!= count_angle && i != 0)
		{
			for(int j=1;j<count_angle*2;j++)
			{
				glm::vec3* new_vertices2 = new glm::vec3(new_vertices1->x, sin(angle_base * j + M_PI / 2.f) * new_vertices1->y, cos(angle_base * j + M_PI / 2.f) * new_vertices1->y);
				vertices.push_back(new_vertices2);
			}
		}
	}

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
	/*
	*/
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

Capsule::Capsule(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r)
	:position{ glm::vec3(px, py, pz) }, pointA{ glm::vec3(pAx, pAy, pAz) }, pointB{ glm::vec3(pBx, pBy, pBz) }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_CapsuleS++;
}
Capsule::Capsule(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r)
	:position{ pos }, pointA{ pA }, pointB{ pB }, radius{ r }, fixed{ false }, drawable{ false }
{
	COUNT_CapsuleS++;
}
Capsule::Capsule(float px, float py, float pz, float pAx, float pAy, float pAz, float pBx, float pBy, float pBz, float r, bool f)
	:position{ glm::vec3(px, py, pz) }, pointA{ glm::vec3(pAx, pAy, pAz) }, pointB{ glm::vec3(pBx, pBy, pBz) }, radius{ r }, fixed{ f }, drawable{ false }
{
	COUNT_CapsuleS++;
}
Capsule::Capsule(glm::vec3 pos, glm::vec3 pA, glm::vec3 pB, float r, bool f)
	:position{ pos }, pointA{ pA }, pointB{ pB }, radius{ r }, fixed{ f }, drawable{ false }
{
	COUNT_CapsuleS++;
}

Capsule::~Capsule()
{
	if (drawable)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
	COUNT_CapsuleS--;
}

void Capsule::CreateDrawableModel()
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
void Capsule::Draw()
{
	if (drawable)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertex_count);
	}
}
std::vector<float> Capsule::CreateDataModel()
{
	// variable for data model
	std::vector<float> data;

	std::vector<glm::vec3*> vertices;

	// crating vertices of Capsule

	int count_angle = (radius + 3) * 2;
	float angle_base = M_PI / (float)count_angle;

	float len = glm::length(pointB - pointA);

	for (int i = 0; i <= count_angle; i++)
	{
		if (i > count_angle / 2 && len != 0.0)
		{
			len = 0.0;
			glm::vec3* new_vertices1 = new glm::vec3(cos(angle_base * (i-1)) * radius + len, sin(angle_base * (i - 1)) * radius, 0.0);
			vertices.push_back(new_vertices1);
			if (i != count_angle && i != 0)
			{
				for (int j = 1; j < count_angle * 2; j++)
				{
					glm::vec3* new_vertices2 = new glm::vec3(new_vertices1->x, sin(angle_base * j + M_PI / 2.f) * new_vertices1->y, cos(angle_base * j + M_PI / 2.f) * new_vertices1->y);
					vertices.push_back(new_vertices2);
				}

			}
		}
		glm::vec3* new_vertices1 = new glm::vec3(cos(angle_base * i) * radius + len, sin(angle_base * i) * radius, 0.0);
		vertices.push_back(new_vertices1);
		if (i != count_angle && i != 0)
		{
			for (int j = 1; j < count_angle * 2; j++)
			{
				glm::vec3* new_vertices2 = new glm::vec3(new_vertices1->x, sin(angle_base * j + M_PI / 2.f) * new_vertices1->y, cos(angle_base * j + M_PI / 2.f) * new_vertices1->y);
				vertices.push_back(new_vertices2);
			}
		}
	}

	glm::vec3 d_vec = glm::normalize(pointB - pointA);
	float alfa = glm::atan(d_vec.y, d_vec.x); // z
	float beta = glm::acos(sqrt(d_vec.x * d_vec.x + d_vec.y * d_vec.y)); // y
	if (d_vec.z < 0.0) beta *= -1;
	float gamma = 0.0;// x
	// rotatiom matrix
	glm::mat3x3 rotate_mat = glm::mat3x3(1.0);
	rotate_mat[0][0] = cos(alfa) * cos(beta); rotate_mat[1][0] = -sin(alfa) * cos(gamma) + cos(alfa) * sin(beta) * sin(gamma); rotate_mat[2][0] = -sin(alfa) * sin(gamma) - cos(alfa) * sin(beta) * cos(gamma);
	rotate_mat[0][1] = sin(alfa) * cos(beta); rotate_mat[1][1] = cos(alfa) * cos(gamma) + sin(alfa) * sin(beta) * sin(gamma);  rotate_mat[2][1] = cos(alfa) * sin(gamma) - sin(alfa) * sin(beta) * cos(gamma);
	rotate_mat[0][2] = sin(beta);			  rotate_mat[1][2] = -cos(beta) * sin(gamma);									   rotate_mat[2][2] = cos(beta) * cos(gamma);

	for (glm::vec3* v : vertices)
	{
		glm::vec3 tmp = (rotate_mat * *v);
		v->x = tmp.x;
		v->y = tmp.y;
		v->z = tmp.z;
	}


	//count_angle++;
	/// create data for OpenGL from vertices to TRIANGLES
	for (int i = 1; i < count_angle * 2 + 1; i++)
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
	/*
	*/
	for (int i = 0; i < count_angle - 1; i++)
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
				data.push_back(vertices[j + 1]->x);
				data.push_back(vertices[j + 1]->y);
				data.push_back(vertices[j + 1]->z);
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
				data.push_back(vertices[j + 1]->x);
				data.push_back(vertices[j + 1]->y);
				data.push_back(vertices[j + 1]->z);

				data.push_back(vertices[j + count_angle * 2 + 1]->x);
				data.push_back(vertices[j + count_angle * 2 + 1]->y);
				data.push_back(vertices[j + count_angle * 2 + 1]->z);
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
			data.push_back(vertices[index - 1]->x);
			data.push_back(vertices[index - 1]->y);
			data.push_back(vertices[index - 1]->z);
		}
		else
		{
			data.push_back(vertices[i - 1]->x);
			data.push_back(vertices[i - 1]->y);
			data.push_back(vertices[i - 1]->z);
		}
	}

	// cleaning
	for (auto v : vertices) delete v;
	vertices.clear();

	return data;
}

Ray3d::Ray3d(glm::vec3 pos, glm::vec3 ray) : position{ pos }
{
	this->ray = glm::normalize(ray);
}
Ray3d::Ray3d(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2) : position{ pos }
{
	ray = glm::normalize(p2 - p1);
}
Ray3d::Ray3d(float px, float py, float pz, float rx, float ry, float rz) : position{ glm::vec3(px, py, pz) }, ray{ glm::vec3(rx, ry, rz) }
{
	this->ray = glm::normalize(this->ray);
}

Ray3d::~Ray3d()
{
	if (drawable)
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
	}
}

void Ray3d::CreateDrawableModel()
{
	std::vector<float> data;
	data.push_back(0.0);
	data.push_back(0.0);
	data.push_back(0.0);
	data.push_back(ray.x * 10.0);
	data.push_back(ray.y * 10.0);
	data.push_back(ray.z * 10.0);
	vertex_count = 2;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
	glEnableVertexAttribArray(0);
	drawable = true;
}
void Ray3d::Draw()
{
	if (drawable)
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, vertex_count);
	}
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