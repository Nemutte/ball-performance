cmake_minimum_required(VERSION 3.0.0)

project(ball-performance VERSION 1.0.0)
cmake_policy(SET CMP0072 NEW)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)

add_executable(ball-performance
	src/config.h
	src/config.cpp
	src/main.cpp
	src/main.h
	src/ball.cpp
	src/ball.h
	src/model.cpp
	src/model.h
	src/collision.cpp
	src/collision.h
	src/glad.c
)

target_include_directories(ball-performance PRIVATE dependencies)

target_link_libraries(ball-performance glfw OpenGL::GL)