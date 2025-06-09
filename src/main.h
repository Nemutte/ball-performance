#pragma once

#include "config.h"
#include "model.h"
#include "ball.h"

GLFWwindow* window;
unsigned int shader;
int screen_width = 640;
int screen_height = 480;
float spawn_ball_ratio = 0.3f;

std::vector<Ball*> balls;

bool SetupGLFW();
unsigned int make_shader(const std::string& vertex_filepath, const std::string& fragment_filepath);
unsigned int make_module(const std::string& filepath, unsigned int module_type);
void Draw();
void MoveToBorders();
void Update(float dt);
void Collision();
