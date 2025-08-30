#pragma once

#include "config.h"
#include "model.h"
#include "ball.h"

#define M_PI  3.14159265

hib::Ball testB = hib::Ball(glm::vec3(0, 0, 0), 1.0);

GLFWwindow* window;
unsigned int shader2d;
unsigned int shader3d;
int screen_width = 640;
int screen_height = 480;
float spawn_ball_ratio = 0.3f;

const float mouseSensitivity = 0.002f;  // Czu³oœæ obrotu
float cameraAngle_alfa = 0.f;
float cameraAngle_beta = 0.f;
bool CLOSE = false;

glm::mat4 view;
glm::mat4 projection;

std::vector<Circle*> circles;
std::vector<Ball*> balls;

bool SetupGLFW();
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
unsigned int make_module(const std::string& filepath, unsigned int module_type);
void Draw2d();
void Draw3d();
void Input(GLFWwindow* window, Camera* camera);
void MoveToBorders();
void Update2d(float dt);
void Update3d(float dt);
void Collision();
int StartSimulation2d();
int StartSimulation3d();
